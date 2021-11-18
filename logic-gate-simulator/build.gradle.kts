plugins {
    java
    id("org.openjfx.javafxplugin") version "0.0.10"
}

group = "com.github.jacekpoz"
version = "0.0.1"

repositories {
    mavenCentral()
}

dependencies {
    testImplementation("org.junit.jupiter:junit-jupiter-api:5.8.1")
    testRuntimeOnly("org.junit.jupiter:junit-jupiter-engine:5.8.1")
}

tasks.getByName<Test>("test") {
    useJUnitPlatform()
}

javafx {
    version = "17"
    modules("javafx.controls", "javafx.fxml")
}
