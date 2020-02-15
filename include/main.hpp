#pragma once
#include <map>
#include <vector>

#include "../extern/beatsaber-hook/shared/utils/utils.h"

static struct Config_t {
    bool Lights = true;
    bool Walls = false;
    bool Sabers = false;
    bool Trails = false;
    bool Notes = false;
    bool QSabers = false;
    double SaberASpeed = 1.0;
    double SaberBSpeed = 1.0;
    double SabersStartDiff = 180.0;
    double LightASpeed = 2.0;
    double LightBSpeed = 2.0;
    double LightsStartDiff = 180.0;
    double WallsSpeed = 2.0;
} Config;

class ColorCollection {
    public:
        Color saberAColor;
        Color saberBColor;
        Color environmentColor0;
        Color environmentColor1;
        Color obstaclesColor;
        ColorCollection();
        ColorCollection(Il2CppObject* colorScheme);
        void SetToColorManager(Il2CppObject* colorManager);
    private:
        Il2CppObject* CreateColorSO();
        void SetToColorScheme(Il2CppObject* colorScheme);
        void SetParticleEffects(Array<Il2CppObject*>* particles, Il2CppObject* colorSO0, Il2CppObject* colorSO1);
        void SetLightSwitchEventEffects(Array<Il2CppObject*>* lightSwitches, Il2CppObject* colorSO0, Il2CppObject* colorSO1);
        static void CacheIl2Cpp();
        static bool _cached;
        static FieldInfo* saberAColorField;
        static FieldInfo* saberBColorField;
        static FieldInfo* environmentColor0Field;
        static FieldInfo* environmentColor1Field;
        static FieldInfo* obstaclesColorField;
        static FieldInfo* managerSaberAColorField;
        static FieldInfo* managerSaberBColorField;
        static FieldInfo* managerEnvironmentColor0Field;
        static FieldInfo* managerEnvironmentColor1Field;
        static FieldInfo* managerObstaclesColorField;
        static FieldInfo* managerColorSchemaField;
        static Il2CppObject* particleTypeObject;
        static FieldInfo* particleLightColor0Field;
        static FieldInfo* particleLightColor1Field;
        static Il2CppObject* lightEventTypeObject;
        static FieldInfo* lightEventColor0Field;
        static FieldInfo* lightEventColor1Field;
        static FieldInfo* lightEventHighlightColor0Field;
        static FieldInfo* lightEventHighlightColor1Field;
        static Il2CppClass* simpleColorSOClass;
        static Il2CppObject* simpleColorSOTypeObject;
        static const MethodInfo* simpleColorSO_setColor;
        static const MethodInfo* SO_createInstance;
};
