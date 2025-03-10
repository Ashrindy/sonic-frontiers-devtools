#pragma once
#include <resources/ManagedMemoryRegistry.h>
#include <resources/managed-memory/ManagedCArray.h>
#include <utilities/CompatibleObject.h>

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_rangers
	inline constexpr const char* nodeTypeNames[] = {
		"",
		"Path",
		"PathMotion",
		"Camera",
		"CameraMotion",
		"Character",
		"CharacterMotion",
		"CharacterBehavior",
		"Model",
		"",
		"ModelMotion",
		"ModelNode",
		"Element"
	};

	inline constexpr const char* elementIDStrings[] = {
		"",
		"CameraParams",
		"",
		"DrawOff",
		"",
		"PathOffset",
		"CameraShake",
		"CameraShakeLoop",
		"Effect",
		"DirectionalLight",
		"PathInterpolation",
		"CullingDisable",
		"CameraNearFar",
		"UVAnim",
		"VisibilityAnim",
		"MaterialAnim",
		"MultipleAnim",
		"CameraOffset",
		"ModelFade",
		"DebugMotion",
		"CameraHedgehog",
		"CameraInGame",
		"PointLight",
		"VertexAnimationTexture",
		"Spotlight",
		"ControllerVibration",
		"SpotlightModel",

		"BloomParam",
		"DOFParam",
		"ColorContrast",
		"CameraControlParam",
		"ShadowResolution",
		"GodrayParam",
		"AtmosphereGodrayParam",
		"AtmosphereHeightFogParam",
		"ChromaticAberrationFilterParam",
		"VignetteParam",
		"Fade",
		"LetterBox",
		"ModelClipping",
		"PbaReset",
		"BossName",
		"Caption",
		"Sound",
		"Time",
		"Sun",
		"LookAtIK",
		"CameraBlurParam",
		"GeneralTrigger",
		"FootIk",
		"DitherParam",
		"QTE",
		"FacialAnimation",
		"OverrideASM",
		"Aura",
		"ChangeTimeScale",
		"CyberSpaceNoise",
		"LipAnimation",
		"AuraRoad",
		"MovieView",
		"CrossFade",
		"Weather",
		"ShadowMapParam",
		"VariablePointLight",
		"OpeningLogo",
		"DensitySectorPoint",
		"FxColUpdate",
		"VAT",
		"QTEAccel",
		"TheEndCableObject",
		"RifleBeastLighting"
	};

	constexpr unsigned int hhElementCount = 27;
#endif

#ifdef DEVTOOLS_TARGET_SDK_miller
	inline constexpr const char* nodeTypeNames[] = {
		"",
		"Path",
		"PathMotion",
		"Camera",
		"CameraMotion",
		"Character",
		"CharacterMotion",
		"CharacterBehavior",
		"Model",
		"",
		"ModelMotion",
		"ModelNode",
		"Element"
	};

	inline constexpr const char* elementIDStrings[] = {
		"",
		"CameraParams",
		"",
		"DrawOff",
		"",
		"PathOffset",
		"CameraShake",
		"CameraShakeLoop",
		"Effect",
		"DirectionalLight",
		"PathInterpolation",
		"CullingDisable",
		"CameraNearFar",
		"UVAnim",
		"VisibilityAnim",
		"MaterialAnim",
		"MultipleAnim",
		"CameraOffset",
		"ModelFade",
		"DebugMotion",
		"CameraHedgehog",
		"CameraInGame",
		"PointLight",
		"VertexAnimationTexture",
		"Spotlight",
		"ControllerVibration",
		"TexturePatternAnim",
		"MaterialParam",

		"BloomParam",
		"DOFParam",
		"ColorContrast",
		"CameraControlParam",
		"ShadowResolution",
		"SSAOParam",
		"OcclusionCapsuleParam",
		"GodrayParam",
		"AtmosphereGodrayParam",
		"AtmosphereHeightFogParam",
		"ChromaticAberrationFilterParam",
		"VignetteParam",
		"Fade",
		"LetterBox",
		"ModelClipping",
		"PbaReset",
		"BossName",
		"Caption",
		"Sound",
		"Time",
		"Sun",
		"LookAtIK",
		"CameraBlurParam",
		"GeneralTrigger",
		"FootIk",
		"DitherParam",
		"QTE",
		"FacialAnimation",
		"OverrideASM",
		"Aura",
		"ChangeTimeScale",
		"CyberSpaceNoise",
		"LipAnimation",
		"AuraRoad",
		"MovieView",
		"CrossFade",
		"Weather",
		"ShadowMapParam",
		"VariablePointLight",
		"OpeningLogo",
		"DensitySectorPoint",
		"BulletTime",
		"TimeStop",
		"ObjectTimeStop",
		"ShadowShape",
		"Falloff",
		"Fog",
		"DOF",
		"FxColUpdate"
	};

	constexpr unsigned int hhElementCount = 28;
#endif

	class Context : public CompatibleObject {
		void ContainsElement(bool& contains, const unsigned int& elementId, hh::dv::DvNodeBase* node);
	public:
		using CompatibleObject::CompatibleObject;

		hh::fnd::Reference<hh::dv::DvSceneControl> goDVSC{};
		app::evt::EventScene* evtScene;
		std::mt19937 mt{ std::random_device{}() };

		csl::ut::VariableString cutsceneName;
		csl::ut::VariableString nodeName;
		int nodeType;
		int elementId;

		bool ContainsElement(const unsigned int& elementId);
		static void GetChildren(hh::dv::DvNodeBase* node, csl::ut::MoveArray32<hh::dv::DvNodeBase*>& value, bool& includeElements);
		static void GetNodes(hh::dv::DvSceneNodeTree* nodeTree, csl::ut::MoveArray32<hh::dv::DvNodeBase*>& value, bool includeElements);
		hh::dv::DvNodeBase* CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId, hh::dv::DvNodeBase* parent);
		void ParentNode(hh::dv::DvNodeBase* parent, hh::dv::DvNodeBase* child);

		Context(csl::fnd::IAllocator* allocator);
	};
}
