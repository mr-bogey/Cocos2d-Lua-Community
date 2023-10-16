plugins {
    alias(libs.plugins.library)
}

android {
    namespace = "org.cocos2dx"
    compileSdk = 34

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
        java.setSrcDirs(listOf("/src"))
        manifest.srcFile("AndroidManifest.xml")
    }
}

dependencies {
    implementation(libs.appcompat)
}
