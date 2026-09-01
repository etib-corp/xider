/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <iostream>
#include <memory>

#include <guillaume/ecs/entity_registry_container.hpp>

namespace
{

	class DummyEntity: public guillaume::ecs::Entity
	{
		public:
		void initialize(void) override
		{
		}
		void update(void) override
		{
		}
	};

}	 // namespace

int main(void)
{
	guillaume::ecs::EntityRegistryContainer registry;

	// Add entities to the registry.
	for (std::size_t i = 0; i < 10; ++i) {
		auto entity = std::make_shared<DummyEntity>();
		guillaume::ecs::Entity::Signature signature;
		signature.set(i % 4);
		entity->setSignature(signature);
		registry.addEntity(std::move(entity));
	}

	std::cout << "Added 10 entities to the registry.\n";

	// Query entities by signature.
	guillaume::ecs::Entity::Signature signature;
	signature.set(2);
	auto matches = registry.getEntityWithSignature(signature);
	std::cout << "Entities matching signature 2: " << matches.size() << "\n";

	return 0;
}
