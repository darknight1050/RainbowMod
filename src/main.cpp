#include "../include/main.hpp"

rapidjson::Document& config_doc = Configuration::config;

bool isInTutorial = false; 

float saberAColorHue = 0;
float saberBColorHue = 0;
float environmentColor0Hue = 0;
float environmentColor1Hue = 0;
float obstaclesColorHue = 0;

Color saberAColor;
Color saberBColor;
Color environmentColor0;
Color environmentColor1;
Color obstaclesColor;

Il2CppObject* simpleColorSO0 = nullptr;
Il2CppObject* simpleColorSO1 = nullptr;

std::map<Il2CppObject*, std::vector<Il2CppObject*>> sabersMaterials;
Array<Il2CppObject*>* colorManagers = nullptr;
Array<Il2CppObject*>* lightSwitchEventEffects = nullptr;

Il2CppObject* basicSaberModelControllers[2];

Color ColorFromHSV(float h, float s, float v){
    h/=360.0f;
    Color color;
    il2cpp_utils::RunMethod(&color, "UnityEngine", "Color", "HSVToRGB", h, s, v);
    return color;
}

Color GetLinearColor(Color color){
    Color linearColor;
    il2cpp_utils::RunMethod(&linearColor.r, "UnityEngine", "Mathf", "GammaToLinearSpace", color.r);
    il2cpp_utils::RunMethod(&linearColor.g, "UnityEngine", "Mathf", "GammaToLinearSpace", color.g);
    il2cpp_utils::RunMethod(&linearColor.b, "UnityEngine", "Mathf", "GammaToLinearSpace", color.b);
    linearColor.a = color.a;
    return linearColor;
}

Array<Il2CppObject*>* GetAllObjectsOfType(std::string_view nameSpace, std::string_view klassName) {
    Array<Il2CppObject*>* objects;
    il2cpp_utils::RunMethod(&objects, "UnityEngine", "Resources", "FindObjectsOfTypeAll", il2cpp_utils::GetSystemType(nameSpace, klassName));
    return objects;
}

Color GetColorFromManager(Il2CppObject* colorManager, const char* fieldName){
    Color color;
    il2cpp_utils::RunMethod(&color, il2cpp_utils::GetFieldValue(colorManager, fieldName), "get_color");
    return color;
}

Il2CppObject* CreateColorSO() {
    Il2CppObject* simpleColorSO;
    il2cpp_utils::RunMethod(&simpleColorSO, "UnityEngine", "ScriptableObject", "CreateInstance", il2cpp_utils::GetSystemType("", "SimpleColorSO"));
    return simpleColorSO;
}

void CacheSaberMaterials(Il2CppObject* saber){
    std::vector<Il2CppObject*> materials; 
    Il2CppClass* shaderClass = il2cpp_utils::GetClassFromName("UnityEngine", "Shader");
    int glowID, bloomID;
    il2cpp_utils::RunMethod(&glowID, shaderClass, "PropertyToID", il2cpp_utils::createcsstr("_Glow"));
    il2cpp_utils::RunMethod(&bloomID, shaderClass, "PropertyToID", il2cpp_utils::createcsstr("_Bloom"));
    Array<Il2CppObject*>* childTransforms;
    il2cpp_utils::RunMethod(&childTransforms, saber, "GetComponentsInChildren", il2cpp_utils::GetSystemType("UnityEngine", "Transform"), false);
    for (int i= 0; i< childTransforms->Length(); i++)
    {
        Array<Il2CppObject*>* renderers;
        il2cpp_utils::RunMethod(&renderers, childTransforms->values[i], "GetComponentsInChildren", il2cpp_utils::GetSystemType("UnityEngine", "Renderer"), false);
        for (int j = 0; j < renderers->Length(); j++)
        {
            Array<Il2CppObject*>* sharedMaterials;
            il2cpp_utils::RunMethod(&sharedMaterials, renderers->values[j], "get_sharedMaterials");
            for (int h = 0; h < sharedMaterials->Length(); h++)
            {
                Il2CppObject* material = sharedMaterials->values[h];
                bool setColor = false;
                bool hasGlow;
                il2cpp_utils::RunMethod(&hasGlow, material, "HasProperty", glowID);
                if (hasGlow)
                {
                    float glowFloat;
                    il2cpp_utils::RunMethod(&glowFloat, material, "GetFloat", glowID);
                    if (glowFloat > 0)
                       setColor = true;
                }
                if (!setColor)
                {
                    bool hasBloom;
                    il2cpp_utils::RunMethod(&hasBloom, material, "HasProperty", bloomID);
                    if (hasBloom)
                    {
                        float bloomFloat;
                        il2cpp_utils::RunMethod(&bloomFloat, material, "GetFloat", bloomID);
                        if (bloomFloat > 0)
                            setColor = true;
                    }
                }
                if (setColor)
                {
                    materials.push_back(material); 
                }
            }
        }
    }
    if(materials.size() > 0)
        sabersMaterials[saber] = materials;
}

void SetSaberColor(Il2CppObject* saber, Color color){
    std::map<Il2CppObject*, std::vector<Il2CppObject*>>::iterator it = sabersMaterials.find(saber);
    if(it == sabersMaterials.end())
    {
        CacheSaberMaterials(saber);
    }
    for (Il2CppObject* material : sabersMaterials[saber]) 
    {
        il2cpp_utils::RunMethod(material, "SetColor", il2cpp_utils::createcsstr("_Color"), color);
    }
}

MAKE_HOOK_OFFSETLESS(SceneManager_SetActiveScene, bool, int scene)
{
    saberAColor = { 168.0f/255.0f, 32.0f/255.0f, 32.0f/255.0f }; //RED
    saberBColor = { 32.0f/255.0f, 100.0f/255.0f, 168.0f/255.0f }; //BLUE
    return  SceneManager_SetActiveScene(scene);
}

MAKE_HOOK_OFFSETLESS(TutorialController_Start, void, Il2CppObject* self){
    TutorialController_Start(self);
    isInTutorial = true;
}

MAKE_HOOK_OFFSETLESS(TutorialController_OnDestroy, void, Il2CppObject* self){
    TutorialController_OnDestroy(self);
    isInTutorial = false;
}

MAKE_HOOK_OFFSETLESS(ColorManager_ColorForNoteType, Color, Il2CppObject* self, NoteType type){
    if(Config.Notes){
        return type == NoteA ? saberAColor : saberBColor;
    }
    return ColorManager_ColorForNoteType(self, type);
}

MAKE_HOOK_OFFSETLESS(ColorManager_ColorForSaberType, Color, Il2CppObject* self, SaberType type){
    if(Config.Sabers){
        return type == SaberA ? saberAColor : saberBColor;
    }
    return ColorManager_ColorForSaberType(self, type);
}

MAKE_HOOK_OFFSETLESS(ColorManager_EffectsColorForSaberType, Color, Il2CppObject* self, SaberType type){
    if(Config.Sabers){
        return type == SaberA ? saberAColor : saberBColor;
    }
    return ColorManager_EffectsColorForSaberType(self, type);
}

MAKE_HOOK_OFFSETLESS(ColorManager_GetObstacleEffectColor, Color, Il2CppObject* self){
    if(Config.Walls){
        return obstaclesColor;
    }
    return ColorManager_GetObstacleEffectColor(self);
}

MAKE_HOOK_OFFSETLESS(SaberManager_RefreshSabers, void, Il2CppObject* self){
    colorManagers = GetAllObjectsOfType("", "ColorManager");
    lightSwitchEventEffects = GetAllObjectsOfType("", "LightSwitchEventEffect");
    if(simpleColorSO0 == nullptr || simpleColorSO1 == nullptr){
        simpleColorSO0 = CreateColorSO();
        simpleColorSO1 = CreateColorSO();
    }
    SaberManager_RefreshSabers(self);
}

MAKE_HOOK_OFFSETLESS(SaberManager_Update, void, Il2CppObject* self){
    if(colorManagers != nullptr) {
        if(isInTutorial){
            if(colorManagers->Length() > 0){
                Il2CppObject* colorManager = colorManagers->values[colorManagers->Length()-1];
                saberAColor = GetColorFromManager(colorManager, "_saberAColor");
                saberBColor = GetColorFromManager(colorManager, "_saberBColor");
                environmentColor0 = GetColorFromManager(colorManager, "_environmentColor0");
                environmentColor1 = GetColorFromManager(colorManager, "_environmentColor1");
                obstaclesColor = GetColorFromManager(colorManager, "_obstaclesColor");
            }
        }else{
            saberAColorHue = fmod(saberAColorHue+Config.SaberASpeed, 360);
            saberBColorHue = fmod(saberBColorHue+Config.SaberBSpeed, 360);
            saberAColor = ColorFromHSV(saberAColorHue, 1.0f, 1.0f);
            saberBColor = ColorFromHSV(saberBColorHue, 1.0f, 1.0f);

            environmentColor0Hue = fmod(environmentColor0Hue+Config.LightASpeed, 360);
            environmentColor1Hue = fmod(environmentColor1Hue+Config.LightBSpeed, 360);
            environmentColor0 = ColorFromHSV(environmentColor0Hue, 1.0f, 1.0f);
            environmentColor1 = ColorFromHSV(environmentColor1Hue, 1.0f, 1.0f);

            obstaclesColorHue = fmod(obstaclesColorHue + Config.WallsSpeed, 360);
            obstaclesColor = ColorFromHSV(obstaclesColorHue, 1.0f, 1.0f);
            
            for (int i = 0; i < colorManagers->Length(); i++) {
                Il2CppObject* colorsDidChangeEvent = il2cpp_utils::GetFieldValue(colorManagers->values[i], "colorsDidChangeEvent");
                if(colorsDidChangeEvent != nullptr)
                    il2cpp_utils::RunMethod(colorsDidChangeEvent, "Invoke");
            }
    
            if(Config.Trails){
                for(int i = 0; i < 2; i++){
                    Color saberColor = i == SaberA ? saberAColor : saberBColor;
                    Il2CppObject* basicSaberModelController = basicSaberModelControllers[i];
                    Il2CppObject* saberWeaponTrail;
                    il2cpp_utils::GetFieldValue(&saberWeaponTrail, basicSaberModelController, "_saberWeaponTrail");
                    il2cpp_utils::RunMethod(saberWeaponTrail, "set_color", GetLinearColor(saberColor));
                    Il2CppObject* light;
                    il2cpp_utils::GetFieldValue(&light, basicSaberModelController, "_light");
                    il2cpp_utils::RunMethod(light, "set_color", saberColor);
                }
            }
            if(Config.Sabers && Config.QSabers) {
                SetSaberColor(il2cpp_utils::GetFieldValue(self, "_leftSaber"), saberAColor);
                SetSaberColor(il2cpp_utils::GetFieldValue(self, "_rightSaber"), saberBColor);
            }
            if(Config.Lights){
                il2cpp_utils::RunMethod(simpleColorSO0, "SetColor", environmentColor0);
                il2cpp_utils::RunMethod(simpleColorSO1, "SetColor", environmentColor1);
                for (int i = 0; i < lightSwitchEventEffects->Length(); i++) {
                    Il2CppObject* lightSwitchEventEffect = lightSwitchEventEffects->values[i];
                    if(lightSwitchEventEffect != nullptr){
                        il2cpp_utils::SetFieldValue(lightSwitchEventEffect, "_lightColor0", simpleColorSO0);
                        il2cpp_utils::SetFieldValue(lightSwitchEventEffect, "_lightColor1", simpleColorSO1);
                        il2cpp_utils::SetFieldValue(lightSwitchEventEffect, "_highlightColor0", simpleColorSO0);
                        il2cpp_utils::SetFieldValue(lightSwitchEventEffect, "_highlightColor1", simpleColorSO1);
                    }
                }
            }
        }
    }
    SaberManager_Update(self);
}

MAKE_HOOK_OFFSETLESS(BasicSaberModelController_Init, void, Il2CppObject* self, Il2CppObject* parent, SaberType saberType){
    basicSaberModelControllers[saberType] = self;
    return BasicSaberModelController_Init(self, parent, saberType);
}

MAKE_HOOK_OFFSETLESS(GameNoteController_Update, void, Il2CppObject* self){
    if(Config.Notes){
        Il2CppObject* disappearingArrowController = il2cpp_utils::GetFieldValue(self, "_disappearingArrowController");
        Il2CppObject* colorNoteVisuals = il2cpp_utils::GetFieldValue(disappearingArrowController, "_colorNoteVisuals");
        Il2CppObject* noteController = il2cpp_utils::GetFieldValue(colorNoteVisuals, "_noteController");
        
        Il2CppObject* noteData = il2cpp_utils::GetFieldValue(noteController, "_noteData");
        NoteType noteType;
        il2cpp_utils::RunMethod(&noteType, noteData, "get_noteType"); 

        Color noteColor = noteType == NoteA ? saberAColor : saberBColor;
        il2cpp_utils::SetFieldValue(colorNoteVisuals, "_noteColor", &noteColor); 
        
        float arrowGlowIntensity = il2cpp_utils::GetFieldValueUnsafe<float>(colorNoteVisuals, "_arrowGlowIntensity");
        Color arrowGlowSpriteRendererColor = noteColor;
        arrowGlowSpriteRendererColor.a = arrowGlowIntensity;
        Il2CppObject* arrowGlowSpriteRenderer = il2cpp_utils::GetFieldValue(colorNoteVisuals, "_arrowGlowSpriteRenderer");
        il2cpp_utils::RunMethod(arrowGlowSpriteRenderer, "set_color", arrowGlowSpriteRendererColor); 
        Il2CppObject* circleGlowSpriteRenderer = il2cpp_utils::GetFieldValue(colorNoteVisuals, "_circleGlowSpriteRenderer");
        il2cpp_utils::RunMethod(circleGlowSpriteRenderer, "set_color", noteColor); 
        Array<Il2CppObject*>* materialPropertyBlockControllers = reinterpret_cast<Array<Il2CppObject*>*>(il2cpp_utils::GetFieldValue(colorNoteVisuals, "_materialPropertyBlockControllers"));
        
        for(int i = 0; i < materialPropertyBlockControllers->Length(); i++){
            Il2CppObject* materialPropertyBlockController = materialPropertyBlockControllers->values[i];
            Il2CppObject* materialPropertyBlock;
            il2cpp_utils::RunMethod(&materialPropertyBlock, materialPropertyBlockController, "get_materialPropertyBlock"); 
            Color materialPropertyBlockColor = noteColor;
            materialPropertyBlockColor.a = 1.0f;
            il2cpp_utils::RunMethod(materialPropertyBlock, "SetColor", il2cpp_utils::createcsstr("_Color"), materialPropertyBlockColor);
            il2cpp_utils::RunMethod(materialPropertyBlockController, "ApplyChanges");
        }
    }
    GameNoteController_Update(self);
}

MAKE_HOOK_OFFSETLESS(ObstacleController_Update, void, Il2CppObject* self){
    if(Config.Walls){
        Il2CppObject* stretchableObstacle = il2cpp_utils::GetFieldValue(self, "_stretchableObstacle");

        Il2CppObject* obstacleFrame = il2cpp_utils::GetFieldValue(stretchableObstacle, "_obstacleFrame");
        float width = il2cpp_utils::GetFieldValueUnsafe<float>(obstacleFrame, "width");
        float height = il2cpp_utils::GetFieldValueUnsafe<float>(obstacleFrame, "height");
        float length = il2cpp_utils::GetFieldValueUnsafe<float>(obstacleFrame, "length");

        il2cpp_utils::RunMethod(stretchableObstacle, "SetSizeAndColor", width, height, length, obstaclesColor);
    }
    ObstacleController_Update(self);
}

void SaveConfig() {
    log(INFO, "Saving Configuration...");
    config_doc.RemoveAllMembers();
    config_doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = config_doc.GetAllocator();
    config_doc.AddMember("Lights", Config.Lights, allocator);
    config_doc.AddMember("Walls", Config.Walls, allocator);
    config_doc.AddMember("Sabers", Config.Sabers, allocator);
    config_doc.AddMember("Trails", Config.Trails, allocator);
    config_doc.AddMember("Notes", Config.Notes, allocator);
    config_doc.AddMember("QSabers", Config.QSabers, allocator);
    config_doc.AddMember("SaberASpeed", Config.SaberASpeed, allocator);
    config_doc.AddMember("SaberBSpeed", Config.SaberBSpeed, allocator);
    config_doc.AddMember("SabersStartDiff", Config.SabersStartDiff, allocator);
    config_doc.AddMember("WallsSpeed", Config.WallsSpeed, allocator);
    Configuration::Write();
    log(INFO, "Saved Configuration!");
}

bool LoadConfig() { 
    log(INFO, "Loading Configuration...");
    Configuration::Load();
    bool foundEverything = true;
    if(config_doc.HasMember("Lights") && config_doc["Lights"].IsBool()){
        Config.Lights = config_doc["Lights"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(config_doc.HasMember("Walls") && config_doc["Walls"].IsBool()){
        Config.Walls = config_doc["Walls"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(config_doc.HasMember("Sabers") && config_doc["Sabers"].IsBool()){
        Config.Sabers = config_doc["Sabers"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(config_doc.HasMember("Trails") && config_doc["Trails"].IsBool()){
        Config.Trails = config_doc["Trails"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(config_doc.HasMember("Notes") && config_doc["Notes"].IsBool()){
        Config.Notes = config_doc["Notes"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(config_doc.HasMember("QSabers") && config_doc["QSabers"].IsBool()){
        Config.QSabers = config_doc["QSabers"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(config_doc.HasMember("SaberASpeed") && config_doc["SaberASpeed"].IsDouble()){
        Config.SaberASpeed = config_doc["SaberASpeed"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(config_doc.HasMember("SaberBSpeed") && config_doc["SaberBSpeed"].IsDouble()){
        Config.SaberBSpeed = config_doc["SaberBSpeed"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(config_doc.HasMember("SabersStartDiff") && config_doc["SabersStartDiff"].IsDouble()){
        Config.SabersStartDiff = config_doc["SabersStartDiff"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(config_doc.HasMember("WallsSpeed") && config_doc["WallsSpeed"].IsDouble()){
        Config.WallsSpeed = config_doc["WallsSpeed"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(foundEverything){
        log(INFO, "Loaded Configuration!");
        return true;
    }
    return false;
}

extern "C" void load()
{
    if(!LoadConfig())
        SaveConfig();
    saberBColorHue = Config.SabersStartDiff;
    environmentColor1Hue = Config.LightsStartDiff;
    log(INFO, "Starting RainbowMod installation...");

    INSTALL_HOOK_OFFSETLESS(SceneManager_SetActiveScene, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "SetActiveScene", 1));
    INSTALL_HOOK_OFFSETLESS(TutorialController_Start, il2cpp_utils::FindMethodUnsafe("", "TutorialController", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(TutorialController_OnDestroy, il2cpp_utils::FindMethodUnsafe("", "TutorialController", "OnDestroy", 0));
    INSTALL_HOOK_OFFSETLESS(ColorManager_ColorForNoteType, il2cpp_utils::FindMethodUnsafe("", "ColorManager", "ColorForNoteType", 1));
    INSTALL_HOOK_OFFSETLESS(ColorManager_ColorForSaberType, il2cpp_utils::FindMethodUnsafe("", "ColorManager", "ColorForSaberType", 1));
    INSTALL_HOOK_OFFSETLESS(ColorManager_EffectsColorForSaberType, il2cpp_utils::FindMethodUnsafe("", "ColorManager", "EffectsColorForSaberType", 1));
    INSTALL_HOOK_OFFSETLESS(ColorManager_GetObstacleEffectColor, il2cpp_utils::FindMethodUnsafe("", "ColorManager", "GetObstacleEffectColor", 0));
    INSTALL_HOOK_OFFSETLESS(SaberManager_RefreshSabers, il2cpp_utils::FindMethodUnsafe("", "SaberManager", "RefreshSabers", 0));
    INSTALL_HOOK_OFFSETLESS(SaberManager_Update, il2cpp_utils::FindMethodUnsafe("", "SaberManager", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(BasicSaberModelController_Init, il2cpp_utils::FindMethodUnsafe("", "BasicSaberModelController", "Init", 2));
    INSTALL_HOOK_OFFSETLESS(GameNoteController_Update, il2cpp_utils::FindMethodUnsafe("", "GameNoteController", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(ObstacleController_Update, il2cpp_utils::FindMethodUnsafe("", "ObstacleController", "Update", 0));
    
    log(INFO, "Successfully installed RainbowMod!");
}