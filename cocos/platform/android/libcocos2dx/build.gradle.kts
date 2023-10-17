plugins {
    alias(libs.plugins.library)
}

android {
    namespace = "org.cocos2dx.lib"
    compileSdk = 34
    ndkVersion = "21.4.7075529"

    defaultConfig {
        minSdk = 24
    }

    buildTypes {
        release {
            isMinifyEnabled = true
            proguardFiles(getDefaultProguardFile("proguard-android.txt"), "proguard-rules.pro")
        }
    }

    sourceSets.getByName("main") {
        java.setSrcDirs(listOf("../java/src"))
        manifest.srcFile("../java/AndroidManifest.xml")
    }
}

dependencies {
    implementation(libs.appcompat)
}
