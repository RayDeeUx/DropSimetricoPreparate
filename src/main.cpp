#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/UILayer.hpp>

using namespace geode::prelude;
using namespace keybinds;

class $modify(MyUILayer, UILayer) {
	bool init(GJBaseGameLayer* gjbgl) {
		if (!UILayer::init(gjbgl)) return false;
		Loader::get()->queueInMainThread([this] {
			if (!PlayLayer::get()) return;
			this->addEventListener(
				KeybindSettingPressedEventV3(Mod::get(), "both-players-jump"),
				[this](Keybind const& keybind, bool down, bool repeat, double timestamp) {
					this->queueButtonPL(static_cast<int>(PlayerButton::Jump), down, timestamp);
				}
			);
			this->addEventListener(
				KeybindSettingPressedEventV3(Mod::get(), "both-players-left"),
				[this](Keybind const& keybind, bool down, bool repeat, double timestamp) {
					this->queueButtonPL(static_cast<int>(PlayerButton::Left), down, timestamp);
				}
			);
			this->addEventListener(
				KeybindSettingPressedEventV3(Mod::get(), "both-players-right"),
				[this](Keybind const& keybind, bool down, bool repeat, double timestamp) {
					this->queueButtonPL(static_cast<int>(PlayerButton::Right), down, timestamp);
				}
			);
			/*
			this->defineKeybind("both-players-jump"_spr, [this](const bool down) {
				this->queueButtonPL(static_cast<int>(PlayerButton::Jump), down);
				return ListenerResult::Propagate;
			});
			this->defineKeybind("both-players-left"_spr, [this](const bool down) {
				this->queueButtonPL(static_cast<int>(PlayerButton::Left), down);
				return ListenerResult::Propagate;
			});
			this->defineKeybind("both-players-right"_spr, [this](const bool down) {
				this->queueButtonPL(static_cast<int>(PlayerButton::Right), down);
				return ListenerResult::Propagate;
			});
			*/
		});
		return true;
	}
	// taken from Geode Team's Custom Keybinds
	[[nodiscard]] bool isPaused() const {
		return !this->isCurrentPlayLayer()
			|| PlayLayer::get()->getParent()->getChildByType<PauseLayer>(0) != nullptr
			|| PlayLayer::get()->getChildByType<GJDropDownLayer>(0) != nullptr;
	}
	[[nodiscard]] bool isCurrentPlayLayer() const {
		const auto playLayer = CCScene::get()->getChildByType<PlayLayer>(0);
		return playLayer != nullptr && playLayer == PlayLayer::get() && playLayer->getChildByType<UILayer>(0) == this;
	}
	void queueButtonPL(const int buttonNumber, const bool isDown, const double timestamp) const {
		PlayLayer* pl = PlayLayer::get();
		if (!pl || this->isPaused()) return;
		if (GJGameLevel* level = pl->m_level) {
			if (!level->m_twoPlayerMode) return;
			if (buttonNumber != 1 && !level->isPlatformer()) return;
		}
		pl->queueButton(buttonNumber, isDown, false, timestamp);
		pl->queueButton(buttonNumber, isDown, true, timestamp);
	}
};

class $modify(MyEditorUI, EditorUI) {
	bool init(LevelEditorLayer* lel) {
		if (!EditorUI::init(lel)) return false;
		Loader::get()->queueInMainThread([this] {
			this->addEventListener(
				KeybindSettingPressedEventV3(Mod::get(), "both-players-jump"),
				[this](Keybind const& keybind, bool down, bool repeat, double timestamp) {
					MyEditorUI::queueButtonLEL(static_cast<int>(PlayerButton::Jump), down, timestamp);
				}
			);
			this->addEventListener(
				KeybindSettingPressedEventV3(Mod::get(), "both-players-left"),
				[this](Keybind const& keybind, bool down, bool repeat, double timestamp) {
					MyEditorUI::queueButtonLEL(static_cast<int>(PlayerButton::Left), down, timestamp);
				}
			);
			this->addEventListener(
				KeybindSettingPressedEventV3(Mod::get(), "both-players-right"),
				[this](Keybind const& keybind, bool down, bool repeat, double timestamp) {
					MyEditorUI::queueButtonLEL(static_cast<int>(PlayerButton::Right), down, timestamp);
				}
			);
			/*
			this->defineKeybind("both-players-jump"_spr, [](const bool down) {
				MyEditorUI::queueButtonLEL(static_cast<int>(PlayerButton::Jump), down);
			});
			this->defineKeybind("both-players-left"_spr, [](const bool down) {
				MyEditorUI::queueButtonLEL(static_cast<int>(PlayerButton::Left), down);
			});
			this->defineKeybind("both-players-right"_spr, [](const bool down) {
				MyEditorUI::queueButtonLEL(static_cast<int>(PlayerButton::Right), down);
			});
			*/
		});
		return true;
	}
	static void queueButtonLEL(const int buttonNumber, const bool isDown, const double timestamp) {
		LevelEditorLayer* lel = LevelEditorLayer::get();
		if (!lel || lel->m_playbackMode != PlaybackMode::Playing) return;
		if (GJGameLevel* level = lel->m_level) {
			if (!level->m_twoPlayerMode) return;
			if (buttonNumber != 1 && !level->isPlatformer()) return;
		}
		lel->queueButton(buttonNumber, isDown, false, timestamp);
		lel->queueButton(buttonNumber, isDown, true, timestamp);
	}
};