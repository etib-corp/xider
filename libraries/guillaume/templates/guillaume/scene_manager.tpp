/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "guillaume/scene_manager.hpp"

namespace guillaume
{
    template<InheritFromScene SceneType> void SceneManager::addScene(void)
    {
        std::type_index typeIndex(typeid(SceneType));
        _scenes[typeIndex] =
            std::make_unique<SceneType>(_localStorage, _sessionStorage);
        getLogger().info("Registered scene type: "
                            + std::string(typeid(SceneType).name()));
    }

    template<InheritFromScene SceneType> void SceneManager::switchToScene(void)
    {
        std::type_index typeIndex(typeid(SceneType));
        if (_scenes.find(typeIndex) == _scenes.end()) {
            getLogger().error("Scene switch failed. Scene type is not "
                                "registered: "
                                + std::string(typeid(SceneType).name()));
            throw std::runtime_error("Scene not found in scene manager");
        }
        _activeSceneType = typeIndex;
        getLogger().info("Switched active scene to type: "
                            + std::string(typeid(SceneType).name()));
    }
} // namespace guillaume