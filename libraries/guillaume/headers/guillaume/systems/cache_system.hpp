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

#pragma once

#include <map>
#include <optional>
#include <vector>
#include <concepts>

#include "guillaume/ecs/system_filler.hpp"

namespace guillaume::systems
{

	/**
	 * @brief Concept for types that can be used as a cache key.
	 */
	template<typename T>
	concept CacheKey = std::equality_comparable_with<T, T>;

	/**
	 * @brief Concept for types that can be cached in the CacheSystem.
	 * These types should be moveable.
	 */
	template<typename T>
	concept CacheValue = std::move_constructible<T>;

	/**
	 * @brief Template class for managing a cache of objects.
	 * @tparam KeyType The type of the key used to index the cache.
	 * @tparam ValueType The type of the value being cached.
	 */
	template<CacheKey KeyType, CacheValue ValueType> class CacheSystem
	{
		protected:
		/**
		 * @brief Entry in the cache associating a key with its cached value and
		 * state.
		 */
		struct CacheEntry {
			std::optional<ValueType>
				value;	  ///< Optional cached value, which may be empty if not
						  ///< yet computed
			size_t objectId { 0 };	  ///< Unique identifier for the cached
									  ///< object, used for engine management
			bool used {
				false
			};	  ///< Flag indicating whether the cached entry was used in the
				  ///< current frame or cycle
		};

		protected:
		/**
		 * @brief Map storing the cache entries.
		 */
		std::map<KeyType, CacheEntry> _cache;

		/**
		 * @brief Check if a cached value exists and matches the current
		 * criteria.
		 * @param key The key to look up.
		 * @param currentCriteria The value to compare against the cached value.
		 * @return True if the key exists and the cached value is equal to
		 * currentCriteria.
		 */
		bool isValid(const KeyType &key, const ValueType &currentCriteria)
		{
			auto it = _cache.find(key);
			if (it != _cache.end()) {
				return it->second.value.has_value()
					&& it->second.value.value() == currentCriteria;
			}
			return false;
		}

		/**
		 * @brief Retrieve an entry from the cache.
		 * @param key The cache key.
		 * @return Pointer to the cache entry, or nullptr if not found.
		 */
		CacheEntry *getEntry(const KeyType &key)
		{
			auto it = _cache.find(key);
			if (it != _cache.end()) {
				return &it->second;
			}
			return nullptr;
		}

		/**
		 * @brief Create or retrieve an entry for a key.
		 * @param key The cache key.
		 * @return Reference to the cache entry.
		 */
		CacheEntry &getOrCreateEntry(const KeyType &key)
		{
			return _cache[key];
		}

		/**
		 * @brief Clear the cache.
		 */
		void clearCache()
		{
			_cache.clear();
		}

		/**
		 * @brief Mark an entry as used.
		 * @param key The cache key.
		 */
		void markAsUsed(const KeyType &key)
		{
			auto entry = getEntry(key);
			if (entry) {
				entry->used = true;
			}
		}

		/**
		 * @brief Mark an entry as unused.
		 * @param key The cache key.
		 */
		void markAsUnused(const KeyType &key)
		{
			auto entry = getEntry(key);
			if (entry) {
				entry->used = false;
			}
		}
	};

}	 // namespace guillaume::systems
