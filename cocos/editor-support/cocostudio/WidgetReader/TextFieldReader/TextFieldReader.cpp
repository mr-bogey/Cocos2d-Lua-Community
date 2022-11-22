/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/



#include "editor-support/cocostudio/WidgetReader/TextFieldReader/TextFieldReader.h"

//#include "ui/UITextField.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "platform/CCFileUtils.h"
#include "editor-support/cocostudio/CocoLoader.h"
#include "editor-support/cocostudio/CSParseBinary_generated.h"
#include "editor-support/cocostudio/LocalizationManager.h"

#include "tinyxml2.h"
#include "flatbuffers/flatbuffers.h"

USING_NS_CC;
using namespace ui;
using namespace flatbuffers;

namespace cocostudio
{
    static TextFieldReader* instanceTextFieldReader = nullptr;
    
    static const char* P_PlaceHolder = "placeHolder";
    static const char* P_Text = "text";
    static const char* P_FontSize = "fontSize";
    static const char* P_FontName = "fontName";
    static const char* P_TouchSizeWidth = "touchSizeWidth";
    static const char* P_TouchSizeHeight = "touchSizeHeight";
    static const char* P_MaxLengthEnable = "maxLengthEnable";
    static const char* P_MaxLength = "maxLength";
    static const char* P_PasswordEnable = "passwordEnable";
    static const char* P_PasswordStyleText = "passwordStyleText";
    
    IMPLEMENT_CLASS_NODE_READER_INFO(TextFieldReader)
    
    TextFieldReader::TextFieldReader()
    {
        
    }
    
    TextFieldReader::~TextFieldReader()
    {
        
    }
    
    TextFieldReader* TextFieldReader::getInstance()
    {
        if (!instanceTextFieldReader)
        {
            instanceTextFieldReader = new (std::nothrow) TextFieldReader();
        }
        return instanceTextFieldReader;
    }
    
    void TextFieldReader::destroyInstance()
    {
        CC_SAFE_DELETE(instanceTextFieldReader);
    }
    
    void TextFieldReader::setPropsFromBinary(cocos2d::ui::Widget *widget, CocoLoader *cocoLoader, stExpCocoNode* cocoNode)
    {
        this->beginSetBasicProperties(widget);
        
        EditBox* editBox = static_cast<EditBox*>(widget);
        
        stExpCocoNode *stChildArray = cocoNode->GetChildArray(cocoLoader);
        
        for (int i = 0; i < cocoNode->GetChildNum(); ++i) {
            std::string key = stChildArray[i].GetName(cocoLoader);
            std::string value = stChildArray[i].GetValue(cocoLoader);
            
            //read all basic properties of widget
            CC_BASIC_PROPERTY_BINARY_READER
            //read all color related properties of widget
            CC_COLOR_PROPERTY_BINARY_READER
            
            else if(key == P_PlaceHolder){
                editBox->setPlaceHolder(value.c_str());
            }else if(key == P_Text){
                editBox->setText(value.c_str());
            }else if(key == P_FontSize){
                editBox->setFontSize(valueToInt(value));
                editBox->setPlaceholderFontSize(valueToInt(value));
            }else if(key == P_FontName){
                editBox->setFontPath(value.c_str());
                editBox->setPlaceholderFontPath(value.c_str());
            }else if(key == P_MaxLength){
                editBox->setMaxLength(valueToInt(value));
            }else if(key == P_PasswordEnable){
                editBox->setInputFlag(valueToBool(value) ? EditBox::InputFlag::PASSWORD : EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
            }
        } //end of for loop
        this->endSetBasicProperties(widget);
    }
    
    void TextFieldReader::setPropsFromJsonDictionary(Widget *widget, const rapidjson::Value &options)
    {
        WidgetReader::setPropsFromJsonDictionary(widget, options);
        
        
        EditBox* editBox = static_cast<EditBox*>(widget);
        bool ph = DICTOOL->checkObjectExist_json(options, P_PlaceHolder);
        if (ph)
        {
            editBox->setPlaceHolder(DICTOOL->getStringValue_json(options, P_PlaceHolder,"input words here"));
        }
        editBox->setText(DICTOOL->getStringValue_json(options, P_Text,"Edit box"));
       
        editBox->setFontSize(DICTOOL->getIntValue_json(options, P_FontSize,20));
        
        int maxLength = DICTOOL->getIntValue_json(options, P_MaxLength,10);
        editBox->setMaxLength(maxLength);

        bool passwordEnable = DICTOOL->getBooleanValue_json(options, P_PasswordEnable);

        editBox->setInputFlag(passwordEnable ? EditBox::InputFlag::PASSWORD : EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
        
        WidgetReader::setColorPropsFromJsonDictionary(widget, options);
    }        
    
    Offset<Table> TextFieldReader::createOptionsWithFlatBuffers(const tinyxml2::XMLElement *objectData,
                                                                flatbuffers::FlatBufferBuilder *builder)
    {
        auto temp = WidgetReader::getInstance()->createOptionsWithFlatBuffers(objectData, builder);
        auto widgetOptions = *(Offset<WidgetOptions>*)(&temp);
        
        std::string path = "";
        std::string plistFile = "";
        int resourceType = 0;
        
        std::string fontName = "";
        int fontSize = 20;
        std::string text = "";
        bool isLocalized = false;
        std::string placeHolder = "Edit Box";
        bool passwordEnabled = false;
        std::string passwordStyleText = "*";
        bool maxLengthEnabled = false;
        int maxLength = 10;
        int areaWidth = 0;
        int areaHeight = 0;
        bool isCustomSize = false;
        
        
        // attributes
        const tinyxml2::XMLAttribute* attribute = objectData->FirstAttribute();
        while (attribute)
        {
            std::string name = attribute->Name();
            std::string value = attribute->Value();
            
            if (name == "PlaceHolderText")
            {
                placeHolder = value;
            }
            else if (name == "LabelText")
            {
                text = value;
            }
            else if (name == "IsLocalized")
            {
                isLocalized = (value == "True") ? true : false;
            }
            else if (name == "FontSize")
            {
                fontSize = atoi(value.c_str());
            }
            else if (name == "FontName")
            {
                fontName = value;
            }
            else if (name == "MaxLengthEnable")
            {
                maxLengthEnabled = (value == "True") ? true : false;
            }
            else if (name == "MaxLengthText")
            {
                maxLength = atoi(value.c_str());
            }
            else if (name == "PasswordEnable")
            {
                passwordEnabled = (value == "True") ? true : false;
            }
            else if (name == "PasswordStyleText")
            {
                passwordStyleText = value;
            }
            else if (name == "IsCustomSize")
            {
                isCustomSize = (value == "True") ? true : false;
            }
            
            
            attribute = attribute->Next();
        }
        
        // child elements
        const tinyxml2::XMLElement* child = objectData->FirstChildElement();
        while (child)
        {
            std::string name = child->Name();
            
            if (name == "FontResource")
            {
                attribute = child->FirstAttribute();
                
                while (attribute)
                {
                    name = attribute->Name();
                    std::string value = attribute->Value();
                    
                    if (name == "Path")
                    {
                        path = value;
                    }
                    else if (name == "Type")
                    {
                        resourceType = 0;
                    }
                    else if (name == "Plist")
                    {
                        plistFile = value;
                    }
                    
                    attribute = attribute->Next();
                }
            }
            
            child = child->NextSiblingElement();
        }
        
        auto options = CreateTextFieldOptions(*builder,
                                              widgetOptions,
                                              CreateResourceData(*builder,
                                                                 builder->CreateString(path),
                                                                 builder->CreateString(plistFile),
                                                                 resourceType),
                                              builder->CreateString(fontName),
                                              fontSize,
                                              builder->CreateString(text),
                                              builder->CreateString(placeHolder),
                                              passwordEnabled,
                                              builder->CreateString(passwordStyleText),
                                              maxLengthEnabled,
                                              maxLength,
                                              areaWidth,
                                              areaHeight,
                                              isCustomSize,
                                              isLocalized);
        
        return *(Offset<Table>*)(&options);
    }
    
    void TextFieldReader::setPropsWithFlatBuffers(cocos2d::Node *node, const flatbuffers::Table *textFieldOptions)
    {
        EditBox* editBox = static_cast<EditBox*>(node);
        auto options = (TextFieldOptions*)textFieldOptions;
        
        std::string placeholder = options->placeHolder()->c_str();
        editBox->setPlaceHolder(placeholder.c_str());
        
        std::string text = options->text()->c_str();
        bool isLocalized = options->isLocalized() != 0;
        if (isLocalized)
        {
            ILocalizationManager* lm = LocalizationHelper::getCurrentManager();
            std::string localizedTxt = lm->getLocalizationString(text);
            std::string::size_type newlineIndex = localizedTxt.find('\n');
            if (newlineIndex != std::string::npos)
                localizedTxt = localizedTxt.substr(0, newlineIndex);
            editBox->setText(localizedTxt.c_str());
        }
        else
        {
            editBox->setText(text.c_str());
        }
        
        int fontSize = options->fontSize();
        editBox->setFontSize(fontSize);
        editBox->setPlaceholderFontSize(fontSize);
        
        std::string fontName = options->fontName()->c_str();
        editBox->setFontPath(fontName);
        editBox->setPlaceholderFontPath(fontName);
        
        bool maxLengthEnabled = options->maxLengthEnabled();
        int maxLength = options->maxLength();
        editBox->setMaxLength(maxLengthEnabled ? maxLength : -1);

        bool passwordEnabled = options->passwordEnabled() != 0;
        editBox->setInputFlag(passwordEnabled ? EditBox::InputFlag::PASSWORD : EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
        
        auto widgetReader = WidgetReader::getInstance();
        widgetReader->setPropsWithFlatBuffers(node, (Table*)options->widgetOptions());
        
        editBox->setUnifySizeEnabled(false);
        editBox->ignoreContentAdaptWithSize(false);
        
        editBox->setInputMode(EditBox::InputMode::ANY);
        editBox->setPlaceholderFontColor(Color4B::GRAY);
        auto widgetOptions = options->widgetOptions();
        auto color = widgetOptions->color();
        editBox->setFontColor(Color4B(color->r(), color->g(), color->b(), color->a()));
    }
    
    Node* TextFieldReader::createNodeWithFlatBuffers(const flatbuffers::Table *textFieldOptions)
    {
        auto options = (TextFieldOptions*)textFieldOptions;
        auto widgetOptions = options->widgetOptions();
        Size contentSize(widgetOptions->size()->width(), widgetOptions->size()->height());
        EditBox* editBox = EditBox::create(contentSize, Scale9Sprite::create());
        
        setPropsWithFlatBuffers(editBox, (Table*)textFieldOptions);
        
        return editBox;
    }
    
}
