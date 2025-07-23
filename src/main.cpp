#include "geode.custom-keybinds/include/Keybinds.hpp"
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/UILayer.hpp>

using namespace geode::prelude;
using namespace keybinds;

class $modify(MyUILayer, UILayer) {
	void defineKeybind(const char* id, const std::function<ListenerResult(bool)>& callback) const {
		PlayLayer* pl = PlayLayer::get();
		if (!pl) return;
		pl->addEventListener<InvokeBindFilter>([this, callback](const InvokeBindEvent* event) {
			return callback(event->isDown());
		}, id);
	}
	bool init(GJBaseGameLayer* gjbgl) {
		if (!UILayer::init(gjbgl)) return false;
		Loader::get()->queueInMainThread([this] {
			if (!PlayLayer::get()) return;
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
	void queueButtonPL(const int buttonNumber, const bool isDown) const {
		PlayLayer* pl = PlayLayer::get();
		if (!pl || this->isPaused()) return;
		if (GJGameLevel* level = pl->m_level) {
			if (!level->m_twoPlayerMode) return;
			if (buttonNumber != 1 && !level->isPlatformer()) return;
		}
		pl->queueButton(buttonNumber, isDown, false);
		pl->queueButton(buttonNumber, isDown, true);
	}
};

class $modify(MyEditorUI, EditorUI) {
	void defineKeybind(const std::string& id, const std::function<void(bool)> &callback) {
		this->addEventListener<InvokeBindFilter>([=](const InvokeBindEvent* event) {
			callback(event->isDown());
			return ListenerResult::Propagate;
		}, id);
	}
	bool init(LevelEditorLayer* lel) {
		if (!EditorUI::init(lel)) return false;
		Loader::get()->queueInMainThread([this] {
			this->defineKeybind("both-players-jump"_spr, [](const bool down) {
				MyEditorUI::queueButtonLEL(static_cast<int>(PlayerButton::Jump), down);
			});
			this->defineKeybind("both-players-left"_spr, [](const bool down) {
				MyEditorUI::queueButtonLEL(static_cast<int>(PlayerButton::Left), down);
			});
			this->defineKeybind("both-players-right"_spr, [](const bool down) {
				MyEditorUI::queueButtonLEL(static_cast<int>(PlayerButton::Right), down);
			});
		});
		return true;
	}
	static void queueButtonLEL(const int buttonNumber, const bool isDown) {
		LevelEditorLayer* lel = LevelEditorLayer::get();
		if (!lel || lel->m_playbackMode != PlaybackMode::Playing) return;
		if (GJGameLevel* level = lel->m_level) {
			if (!level->m_twoPlayerMode) return;
			if (buttonNumber != 1 && !level->isPlatformer()) return;
		}
		lel->queueButton(buttonNumber, isDown, false);
		lel->queueButton(buttonNumber, isDown, true);
	}
};

$on_mod(Loaded) {
	BindManager* binds = BindManager::get();
	binds->registerBindable({
		"both-players-jump"_spr,
		"Both Players Jump",
		"Make both players jump.",
		{ Keybind::create(KEY_I) },
		"Play/DropSimetricoPreparate", false
	});
	binds->registerBindable({
		"both-players-left"_spr,
		"Both Players Left",
		"Make both players move to the left.",
		{ Keybind::create(KEY_J) },
		"Play/DropSimetricoPreparate", false
	});
	binds->registerBindable({
		"both-players-right"_spr,
		"Both Players Right",
		"Make both players move to the right.",
		{ Keybind::create(KEY_L) },
		"Play/DropSimetricoPreparate", false
	});
}