#include "../include/main.hpp"

static rapidjson::Document& config_doc = Configuration::config;

static bool isInTutorial = false; 

static ColorCollection defaultColorScheme;
static bool setDefaultColorScheme = false;
static ColorCollection colorScheme;

static float saberA = 0;
static float saberB = 0;
static float environmentColor0 = 0;
static float environmentColor1 = 0;
static float obstaclesColor = 0;

static std::map<Il2CppObject*, std::vector<Il2CppObject*>> sabersMaterials;

Color ColorFromHSB(float hue, float saturation, float brightness){
    hue/=360.0f;
    int r = 0, g = 0, b = 0;
    if (saturation == 0)
    {
        r = g = b = (int)(brightness * 255.0f + 0.5f);
    }
    else
    {
        float h = (hue - (float)floor (hue)) * 6.0f;
        float f = h - (float)floor (h);
        float p = brightness * (1.0f - saturation);
        float q = brightness * (1.0f - saturation * f);
        float t = brightness * (1.0f - (saturation * (1.0f - f)));
        switch ((int)h)
        {
            case 0:
                r = (int)(brightness * 255.0f + 0.5f);
                g = (int)(t * 255.0f + 0.5f);
                b = (int)(p * 255.0f + 0.5f);
                break;
            case 1:
                r = (int)(q * 255.0f + 0.5f);
                g = (int)(brightness * 255.0f + 0.5f);
                b = (int)(p * 255.0f + 0.5f);
                break;
            case 2:
                r = (int)(p * 255.0f + 0.5f);
                g = (int)(brightness * 255.0f + 0.5f);
                b = (int)(t * 255.0f + 0.5f);
                break;
            case 3:
                r = (int)(p * 255.0f + 0.5f);
                g = (int)(q * 255.0f + 0.5f);
                b = (int)(brightness * 255.0f + 0.5f);
                break;
            case 4:
                r = (int)(t * 255.0f + 0.5f);
                g = (int)(p * 255.0f + 0.5f);
                b = (int)(brightness * 255.0f + 0.5f);
                break;
            case 5:
                r = (int)(brightness * 255.0f + 0.5f);
                g = (int)(p * 255.0f + 0.5f);
                b = (int)(q * 255.0f + 0.5f);
                break;
        }
    }
    Color color;
    color.r = r/255.0f;
    color.g = g/255.0f;
    color.b = b/255.0f;
    color.a = 1.0f;
    return color;
}

Color GetColorFromManager(Il2CppObject* colorManager, const char* fieldName){
    Color c;
    il2cpp_utils::RunMethod<Color>(&c, il2cpp_utils::GetFieldValue(colorManager, fieldName), "get_color");
    return c;
}

Il2CppObject* GetFirstObjectOfType(Il2CppClass* klass){
    Array<Il2CppObject*>* objects;
    il2cpp_utils::RunMethod(&objects, il2cpp_utils::GetClassFromName("UnityEngine", "Resources"), "FindObjectsOfTypeAll", il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(klass)));
    if (objects != nullptr)
    {
        return objects->values[0];
    }
    else
    {
        return nullptr;
    }
}

void CacheSaber(Il2CppObject* saber){
    if(!isInTutorial){
        if(Config.QSabers){
            std::vector<Il2CppObject*> materials; 
            bool getInactive = false;
            Il2CppString* glowString = il2cpp_utils::createcsstr("_Glow");
            Il2CppString* bloomString = il2cpp_utils::createcsstr("_Bloom");
            Il2CppClass* shaderClass = il2cpp_utils::GetClassFromName("UnityEngine", "Shader");
            const MethodInfo* shader_PropertyToIDMethod = il2cpp_functions::class_get_method_from_name(shaderClass, "PropertyToID", 1);
            int glowID, bloomID;
            il2cpp_utils::RunMethod(&glowID, nullptr, shader_PropertyToIDMethod, glowString);
            il2cpp_utils::RunMethod(&bloomID, nullptr, shader_PropertyToIDMethod, bloomString);
            Array<Il2CppObject*>* childTransforms;
            il2cpp_utils::RunMethod(&childTransforms, saber, "GetComponentsInChildren", il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("UnityEngine", "Transform"))), &getInactive);
            for (int i= 0; i< childTransforms->Length(); i++)
            {
                Array<Il2CppObject*>* renderers;
                il2cpp_utils::RunMethod(&renderers, childTransforms->values[i], "GetComponentsInChildren", il2cpp_functions::type_get_object(il2cpp_functions::class_get_type(il2cpp_utils::GetClassFromName("UnityEngine", "Renderer"))), &getInactive);
                for (int j = 0; j < renderers->Length(); j++)
                {
                    Array<Il2CppObject*>* sharedMaterials;
                    il2cpp_utils::RunMethod(&sharedMaterials, renderers->values[j], "get_sharedMaterials");
                    for (int h = 0; h < sharedMaterials->Length(); h++)
                    {
                        Il2CppObject* material = sharedMaterials->values[h];
                        bool setColor = false;
                        bool hasGlow;
                        il2cpp_utils::RunMethod(&hasGlow, material, "HasProperty", &glowID);
                        if (hasGlow)
                        {
                            float glowFloat;
                            il2cpp_utils::RunMethod(&glowFloat, material, "GetFloat", &glowID);
                            if (glowFloat > 0)
                                setColor = true;
                        }
                        if (!setColor)
                        {
                            bool hasBloom;
                            il2cpp_utils::RunMethod(&hasBloom, material, "HasProperty", &bloomID);
                            if (hasBloom)
                            {
                                float bloomFloat;
                                il2cpp_utils::RunMethod(&bloomFloat, material, "GetFloat", &bloomID);
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
    }
    
}

void SetSaberColor(Il2CppObject* saber, Color color){
    Il2CppString* colorString = il2cpp_utils::createcsstr("_Color");
    std::map<Il2CppObject*, std::vector<Il2CppObject*>>::iterator it = sabersMaterials.find(saber);
    if(it == sabersMaterials.end())
    {
        CacheSaber(saber);
    }
    for (Il2CppObject* material : sabersMaterials[saber]) 
    {
        il2cpp_utils::RunMethod(material, "SetColor", colorString, &color);
    }
}

MAKE_HOOK_OFFSETLESS(TutorialController_Start, void, Il2CppObject* self){
    TutorialController_Start(self);
    isInTutorial = true;
}

MAKE_HOOK_OFFSETLESS(TutorialController_OnDestroy, void, Il2CppObject* self){
    TutorialController_OnDestroy(self);
    isInTutorial = false;
}

MAKE_HOOK_OFFSETLESS(SaberManager_Update, void, Il2CppObject* self){
    Il2CppObject* colorManager = GetFirstObjectOfType(il2cpp_utils::GetClassFromName("", "ColorManager"));
    if (colorManager != nullptr)
    {
        if(!isInTutorial && Config.QSabers) {
            SetSaberColor(il2cpp_utils::GetFieldValue(self, "_leftSaber"), colorScheme.saberAColor);
            SetSaberColor(il2cpp_utils::GetFieldValue(self, "_rightSaber"), colorScheme.saberBColor);
        }
    }
    SaberManager_Update(self);
}

MAKE_HOOK_OFFSETLESS(SaberBurnMarkSparkles_LateUpdate, void, Il2CppObject* self, void *type){
    Il2CppObject* colorManager = il2cpp_utils::GetFieldValue(self, "_colorManager");
    if(colorManager != nullptr){
        if (!setDefaultColorScheme) {
            Il2CppObject* scheme;
            il2cpp_utils::RunMethod(&scheme, il2cpp_utils::GetFieldValueUnsafe<Il2CppObject*>(colorManager, "_defaultColorScheme"), "get_colorScheme");
            defaultColorScheme = ColorCollection(scheme);
            setDefaultColorScheme = true;
        }
        if(isInTutorial){
            colorScheme.saberAColor = GetColorFromManager(colorManager, "_saberAColor");
            colorScheme.saberBColor = GetColorFromManager(colorManager, "_saberBColor");
            colorScheme.environmentColor0 = GetColorFromManager(colorManager, "_environmentColor0");
            colorScheme.environmentColor1 = GetColorFromManager(colorManager, "_environmentColor1");
            colorScheme.obstaclesColor = GetColorFromManager(colorManager, "_obstaclesColor");
        }else{
            saberA = fmod(saberA+Config.SaberASpeed, 360);
            saberB = fmod(saberB+Config.SaberBSpeed, 360);
            environmentColor0 = fmod(environmentColor0+Config.LightASpeed, 360);
            environmentColor1 = fmod(environmentColor1+Config.LightBSpeed, 360);
            obstaclesColor = fmod(obstaclesColor+Config.WallsSpeed, 360);
            if(Config.Sabers){
                colorScheme.saberAColor = ColorFromHSB(saberA, 1.0, 1.0);
                colorScheme.saberBColor = ColorFromHSB(saberB, 1.0, 1.0);
            }else{
                colorScheme.saberAColor = GetColorFromManager(colorManager, "_saberAColor");
                colorScheme.saberBColor = GetColorFromManager(colorManager, "_saberBColor");
            }
            if(Config.Lights){
                colorScheme.environmentColor0 = ColorFromHSB(environmentColor0, 1.0, 1.0);
                colorScheme.environmentColor1 = ColorFromHSB(environmentColor1, 1.0, 1.0);
            }else{
                colorScheme.environmentColor0 = GetColorFromManager(colorManager, "_environmentColor0");
                colorScheme.environmentColor1 = GetColorFromManager(colorManager, "_environmentColor1");
            }
            if(Config.Walls){
                colorScheme.obstaclesColor = ColorFromHSB(obstaclesColor, 1.0, 1.0);
            }else{
                colorScheme.obstaclesColor = GetColorFromManager(colorManager, "_obstaclesColor");
            }
        }
        colorScheme.SetToColorManager(colorManager);
    }
    SaberBurnMarkSparkles_LateUpdate(self, type);
}

MAKE_HOOK_OFFSETLESS(SaberWeaponTrail_get_color, Color, Il2CppObject* self){
    if(!Config.Sabers && Config.Trails){
        Il2CppObject* saberTypeObject = il2cpp_utils::GetFieldValue(self, "_saberTypeObject");
        int saberType;
        il2cpp_utils::RunMethod(&saberType, saberTypeObject, "get_saberType"); 
        Color multiplierSaberColor = il2cpp_utils::GetFieldValueUnsafe<Color>(self, "_multiplierSaberColor");
        
        Color saberColor = saberType == 1 ? ColorFromHSB(saberB, 1.0, 1.0) : ColorFromHSB(saberA, 1.0, 1.0);

        saberColor.r = powf(saberColor.r * multiplierSaberColor.r, 2.2f);
        saberColor.g = powf(saberColor.g * multiplierSaberColor.g, 2.2f);
        saberColor.b = powf(saberColor.b * multiplierSaberColor.b, 2.2f);
        saberColor.a = saberColor.a * multiplierSaberColor.a;
        return saberColor;
    }
    return SaberWeaponTrail_get_color(self);
}

MAKE_HOOK_OFFSETLESS(GameNoteController_Update, void, Il2CppObject* self){
    if(Config.Sabers){
        Il2CppObject* disappearingArrowController = il2cpp_utils::GetFieldValue(self, "_disappearingArrowController");
        Il2CppObject* colorNoteVisuals = il2cpp_utils::GetFieldValue(disappearingArrowController, "_colorNoteVisuals");
        Il2CppObject* noteController = il2cpp_utils::GetFieldValue(colorNoteVisuals, "_noteController");
        
        Il2CppObject* noteData = il2cpp_utils::GetFieldValue(noteController, "_noteData");
        int noteType;
        il2cpp_utils::RunMethod(&noteType, noteData, "get_noteType"); 

        Color noteColor;
        
        if(Config.Notes){
            noteColor = noteType == 1 ? colorScheme.saberBColor : colorScheme.saberAColor;
            il2cpp_utils::SetFieldValue(colorNoteVisuals, "_noteColor", &noteColor); 
        }else{
            if (setDefaultColorScheme)
                noteColor = noteType == 1 ? defaultColorScheme.saberBColor : defaultColorScheme.saberAColor;
            else {
                GameNoteController_Update(self);
                return;
            }
        } 
        
        float arrowGlowIntensity = il2cpp_utils::GetFieldValueUnsafe<float>(colorNoteVisuals, "_arrowGlowIntensity");
        Color arrowGlowSpriteRendererColor = noteColor;
        arrowGlowSpriteRendererColor.a = arrowGlowIntensity;
        Il2CppObject* arrowGlowSpriteRenderer = il2cpp_utils::GetFieldValue(colorNoteVisuals, "_arrowGlowSpriteRenderer");
        il2cpp_utils::RunMethod(arrowGlowSpriteRenderer, "set_color", &arrowGlowSpriteRendererColor); 
        Il2CppObject* circleGlowSpriteRenderer = il2cpp_utils::GetFieldValue(colorNoteVisuals, "_circleGlowSpriteRenderer");
        il2cpp_utils::RunMethod(circleGlowSpriteRenderer, "set_color", &noteColor); 
        Array<Il2CppObject*>* materialPropertyBlockControllers = reinterpret_cast<Array<Il2CppObject*>*>(il2cpp_utils::GetFieldValue(colorNoteVisuals, "_materialPropertyBlockControllers"));
        
        for(int i = 0;i<materialPropertyBlockControllers->Length();i++){
            Il2CppObject* materialPropertyBlockController = materialPropertyBlockControllers->values[i];
            Il2CppObject* materialPropertyBlock;
            il2cpp_utils::RunMethod(&materialPropertyBlock, materialPropertyBlockController, "get_materialPropertyBlock"); 
            Color materialPropertyBlockColor = noteColor;
            materialPropertyBlockColor.a = 1.0f;
            il2cpp_utils::RunMethod(materialPropertyBlock, "SetColor", il2cpp_utils::createcsstr("_Color"), &materialPropertyBlockColor);
            il2cpp_utils::RunMethod(materialPropertyBlockController, "ApplyChanges");
        }
    }
    GameNoteController_Update(self);
}

MAKE_HOOK_OFFSETLESS(ObstacleController_Update, void, Il2CppObject* self){
    if(Config.Walls){
        Il2CppObject* stretchableObstacle = il2cpp_utils::GetFieldValue(self, "_stretchableObstacle");

        Color color = colorScheme.obstaclesColor;

        Il2CppObject* obstacleFrame = il2cpp_utils::GetFieldValue(stretchableObstacle, "_obstacleFrame");
        float width = il2cpp_utils::GetFieldValueUnsafe<float>(obstacleFrame, "width");
        float height = il2cpp_utils::GetFieldValueUnsafe<float>(obstacleFrame, "height");
        float length = il2cpp_utils::GetFieldValueUnsafe<float>(obstacleFrame, "length");

        il2cpp_utils::RunMethod(stretchableObstacle, "SetSizeAndColor", &width, &height, &length, &color);
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
    saberB = Config.SabersStartDiff;
    environmentColor1 = Config.LightsStartDiff;
    log(INFO, "Starting RainbowMod installation...");

    INSTALL_HOOK_OFFSETLESS(TutorialController_Start, il2cpp_utils::FindMethod("", "TutorialController", "Start", 0));
    INSTALL_HOOK_OFFSETLESS(TutorialController_OnDestroy, il2cpp_utils::FindMethod("", "TutorialController", "OnDestroy", 0));
    INSTALL_HOOK_OFFSETLESS(SaberManager_Update, il2cpp_utils::FindMethod("", "SaberManager", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(SaberBurnMarkSparkles_LateUpdate, il2cpp_utils::FindMethod("", "SaberBurnMarkSparkles", "LateUpdate", 0));
    INSTALL_HOOK_OFFSETLESS(SaberWeaponTrail_get_color, il2cpp_utils::FindMethod("", "SaberWeaponTrail", "get_color", 0));
    INSTALL_HOOK_OFFSETLESS(GameNoteController_Update, il2cpp_utils::FindMethod("", "GameNoteController", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(ObstacleController_Update, il2cpp_utils::FindMethod("", "ObstacleController", "Update", 0));

    log(INFO, "Successfully installed RainbowMod!");
}