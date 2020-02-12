#include "../include/main.hpp"

bool ColorCollection::_cached = false;
FieldInfo* ColorCollection::saberAColorField;
FieldInfo* ColorCollection::saberBColorField;
FieldInfo* ColorCollection::environmentColor0Field;
FieldInfo* ColorCollection::environmentColor1Field;
FieldInfo* ColorCollection::obstaclesColorField;
FieldInfo* ColorCollection::managerSaberAColorField;
FieldInfo* ColorCollection::managerSaberBColorField;
FieldInfo* ColorCollection::managerEnvironmentColor0Field;
FieldInfo* ColorCollection::managerEnvironmentColor1Field;
FieldInfo* ColorCollection::managerObstaclesColorField;
FieldInfo* ColorCollection::managerColorSchemaField;
Il2CppObject* ColorCollection::particleTypeObject;
FieldInfo* ColorCollection::particleLightColor0Field;
FieldInfo* ColorCollection::particleLightColor1Field;
Il2CppObject* ColorCollection::lightEventTypeObject;
FieldInfo* ColorCollection::lightEventColor0Field;
FieldInfo* ColorCollection::lightEventColor1Field;
FieldInfo* ColorCollection::lightEventHighlightColor0Field;
FieldInfo* ColorCollection::lightEventHighlightColor1Field;
Il2CppClass* ColorCollection::simpleColorSOClass;
Il2CppObject* ColorCollection::simpleColorSOTypeObject;
const MethodInfo* ColorCollection::simpleColorSO_setColor;
const MethodInfo* ColorCollection::SO_createInstance;

Array<Il2CppObject*>* GetAllObjectsOfType(Il2CppObject* typeObject) {
    Array<Il2CppObject*>* objects;
    il2cpp_utils::RunMethod(&objects, il2cpp_utils::GetClassFromName("UnityEngine", "Resources"), "FindObjectsOfTypeAll", typeObject);
    if (objects != nullptr)
        return objects;
    return nullptr;
}

Il2CppObject* ColorCollection::CreateColorSO() {
    ColorCollection::CacheIl2Cpp();
    Il2CppObject* out;
    il2cpp_utils::RunMethod(&out, nullptr, ColorCollection::SO_createInstance, ColorCollection::simpleColorSOTypeObject);
    return out;
}

void ColorCollection::CacheIl2Cpp() {
    if (!ColorCollection::_cached) {
        log(DEBUG, "Caching ColorCollection Fields!");
        Il2CppClass* colorSchemeClass = il2cpp_utils::GetClassFromName("", "ColorScheme");
        ColorCollection::saberAColorField = il2cpp_utils::FindField(colorSchemeClass, "_saberAColor");
        ColorCollection::saberBColorField = il2cpp_utils::FindField(colorSchemeClass, "_saberBColor");
        ColorCollection::environmentColor0Field = il2cpp_utils::FindField(colorSchemeClass, "_environmentColor0");
        ColorCollection::environmentColor1Field = il2cpp_utils::FindField(colorSchemeClass, "_environmentColor1");
        ColorCollection::obstaclesColorField = il2cpp_utils::FindField(colorSchemeClass, "_obstaclesColor");
        Il2CppClass* colorManagerClass = il2cpp_utils::GetClassFromName("", "ColorManager");
        ColorCollection::managerSaberAColorField = il2cpp_utils::FindField(colorManagerClass, "_saberAColor");
        ColorCollection::managerSaberBColorField = il2cpp_utils::FindField(colorManagerClass, "_saberBColor");
        ColorCollection::managerEnvironmentColor0Field = il2cpp_utils::FindField(colorManagerClass, "_environmentColor0");
        ColorCollection::managerEnvironmentColor1Field = il2cpp_utils::FindField(colorManagerClass, "_environmentColor1");
        ColorCollection::managerObstaclesColorField = il2cpp_utils::FindField(colorManagerClass, "_obstaclesColor");
        ColorCollection::managerColorSchemaField = il2cpp_utils::FindField(colorManagerClass, "_colorScheme");
        Il2CppClass* particleClass = il2cpp_utils::GetClassFromName("", "ParticleSystemEventEffect");
        ColorCollection::particleTypeObject = il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(particleClass));
        ColorCollection::particleLightColor0Field = il2cpp_utils::FindField(particleClass, "_lightColor0");
        ColorCollection::particleLightColor1Field = il2cpp_utils::FindField(particleClass, "_lightColor1");
        Il2CppClass* lightEventClass = il2cpp_utils::GetClassFromName("", "LightSwitchEventEffect");
        ColorCollection::lightEventTypeObject = il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(lightEventClass));
        ColorCollection::lightEventColor0Field = il2cpp_utils::FindField(particleClass, "_lightColor0");
        ColorCollection::lightEventColor1Field = il2cpp_utils::FindField(particleClass, "_lightColor1");
        ColorCollection::lightEventHighlightColor0Field = il2cpp_utils::FindField(particleClass, "_highlightColor0");
        ColorCollection::lightEventHighlightColor1Field = il2cpp_utils::FindField(particleClass, "_highlightColor1");
        ColorCollection::simpleColorSOClass = il2cpp_utils::GetClassFromName("", "SimpleColorSO");
        ColorCollection::simpleColorSOTypeObject = il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(simpleColorSOClass));
        ColorCollection::simpleColorSO_setColor = il2cpp_utils::FindMethod(ColorCollection::simpleColorSOClass, "SetColor", 1);
        ColorCollection::SO_createInstance = il2cpp_utils::FindMethod("UnityEngine", "ScriptableObject", "CreateInstance", 1);
        ColorCollection::_cached = true;
        log(DEBUG, "Got color collection fields!");
    }
}

void ColorCollection::SetToColorScheme(Il2CppObject* colorScheme) {
    ColorCollection::CacheIl2Cpp();
    il2cpp_utils::SetFieldValue(colorScheme, ColorCollection::saberAColorField, &saberAColor);
    il2cpp_utils::SetFieldValue(colorScheme, ColorCollection::saberBColorField, &saberBColor);
    il2cpp_utils::SetFieldValue(colorScheme, ColorCollection::environmentColor0Field, &environmentColor0);
    il2cpp_utils::SetFieldValue(colorScheme, ColorCollection::environmentColor1Field, &environmentColor1);
    il2cpp_utils::SetFieldValue(colorScheme, ColorCollection::obstaclesColorField, &obstaclesColor);
}

void ColorCollection::SetParticleEffects(Array<Il2CppObject*>* particles, Il2CppObject* colorSO0, Il2CppObject* colorSO1) {
    ColorCollection::CacheIl2Cpp();
    // Create ColorSOs to wrap the colors
    for (int i = 0; i < particles->Length(); i++) {
        il2cpp_utils::SetFieldValue(particles->values[i], ColorCollection::particleLightColor0Field, colorSO0);
        il2cpp_utils::SetFieldValue(particles->values[i], ColorCollection::particleLightColor1Field, colorSO1);
    }
}

void ColorCollection::SetLightSwitchEventEffects(Array<Il2CppObject*>* lightSwitches, Il2CppObject* colorSO0, Il2CppObject* colorSO1) {
    ColorCollection::CacheIl2Cpp();
    for (int i = 0; i < lightSwitches->Length(); i++) {
        il2cpp_utils::SetFieldValue(lightSwitches->values[i], ColorCollection::lightEventColor0Field, colorSO0);
        il2cpp_utils::SetFieldValue(lightSwitches->values[i], ColorCollection::lightEventColor1Field, colorSO1);
        il2cpp_utils::SetFieldValue(lightSwitches->values[i], ColorCollection::lightEventHighlightColor0Field, colorSO0);
        il2cpp_utils::SetFieldValue(lightSwitches->values[i], ColorCollection::lightEventHighlightColor1Field, colorSO1);
    }
}

void ColorCollection::SetToColorManager(Il2CppObject* colorManager) {
    ColorCollection::CacheIl2Cpp();
    Il2CppObject* scheme = il2cpp_utils::GetFieldValueUnsafe<Il2CppObject*>(colorManager, managerColorSchemaField);
    SetToColorScheme(scheme);
    il2cpp_utils::RunMethod(il2cpp_utils::GetFieldValueUnsafe<Il2CppObject*>(colorManager, ColorCollection::managerSaberAColorField), ColorCollection::simpleColorSO_setColor, &saberAColor);
    il2cpp_utils::RunMethod(il2cpp_utils::GetFieldValueUnsafe<Il2CppObject*>(colorManager, ColorCollection::managerSaberBColorField), ColorCollection::simpleColorSO_setColor, &saberBColor);
    Il2CppObject* environmentColorSO0 = il2cpp_utils::GetFieldValueUnsafe<Il2CppObject*>(colorManager, ColorCollection::managerEnvironmentColor0Field);
    Il2CppObject* environmentColorSO1 = il2cpp_utils::GetFieldValueUnsafe<Il2CppObject*>(colorManager, ColorCollection::managerEnvironmentColor1Field);
    il2cpp_utils::RunMethod(environmentColorSO0, ColorCollection::simpleColorSO_setColor, &environmentColor0);
    il2cpp_utils::RunMethod(environmentColorSO1, ColorCollection::simpleColorSO_setColor, &environmentColor1);
    il2cpp_utils::RunMethod(il2cpp_utils::GetFieldValueUnsafe<Il2CppObject*>(colorManager, ColorCollection::managerObstaclesColorField), ColorCollection::simpleColorSO_setColor, &obstaclesColor);
    SetParticleEffects(GetAllObjectsOfType(particleTypeObject), environmentColorSO0, environmentColorSO1);
    SetLightSwitchEventEffects(GetAllObjectsOfType(lightEventTypeObject), environmentColorSO0, environmentColorSO1);
}

ColorCollection::ColorCollection() {
}

ColorCollection::ColorCollection(Il2CppObject* colorScheme) {
    ColorCollection::CacheIl2Cpp();
    saberAColor = il2cpp_utils::GetFieldValueUnsafe<Color>(colorScheme, ColorCollection::saberAColorField);
    saberBColor = il2cpp_utils::GetFieldValueUnsafe<Color>(colorScheme, ColorCollection::saberBColorField);
    environmentColor0 = il2cpp_utils::GetFieldValueUnsafe<Color>(colorScheme, ColorCollection::environmentColor0Field);
    environmentColor1 = il2cpp_utils::GetFieldValueUnsafe<Color>(colorScheme, ColorCollection::environmentColor1Field);
    obstaclesColor = il2cpp_utils::GetFieldValueUnsafe<Color>(colorScheme, ColorCollection::obstaclesColorField);
}