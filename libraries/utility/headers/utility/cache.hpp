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

#include <concepts>
#include <cstddef>
#include <optional>
#include <map>
#include <utility>
#include <vector>

namespace utility
{
	/**
	 * @brief Concept for cache key types.
	 *
	 * A type is considered a valid cache key if it is equality comparable and
	 * can be hashed using std::hash.
	 *
	 * @tparam T The type to check.
	 */
	template<typename T>
	concept CacheKey = std::equality_comparable<T> && requires(T a, T b) {
		{ a < b } -> std::convertible_to<bool>;
	};

	/**
	 * @brief Concept for cache value types.
	 *
	 * A type is considered a valid cache value if it is movable.
	 *
	 * @tparam T The type to check.
	 */
	template<typename T>
	concept CacheValue = std::movable<T>;

	/**
	 * @brief A generic cache system that stores key-value pairs.
	 *
	 * This class provides a simple caching mechanism for storing and retrieving
	 * values associated with keys. It supports basic operations such as
	 * insertion, retrieval, and removal of cache entries.
	 *
	 * @tparam KeyType The type of the keys used in the cache. Must satisfy the
	 * CacheKey concept.
	 * @tparam ValueType The type of the values stored in the cache. Must
	 * satisfy the CacheValue concept.
	 */
	template<CacheKey KeyType, CacheValue ValueType> class Cache
	{
		public:
		public:
		/**
		 * @brief Represents a single entry in the cache.
		 *
		 * Each cache entry contains an optional value, an object ID, and a
		 * flag indicating whether the entry has been used.
		 */
		struct CacheEntry {
			std::optional<ValueType>
				value {};	 ///< The cached value, if present.
			bool used {
				false
			};	  ///< Indicates whether the entry has been used.

			/**
			 * @brief Checks if the cache entry has a value.
			 *
			 * @return true if the entry has a value, false otherwise.
			 */
			[[nodiscard]] bool hasValue() const noexcept
			{
				return value.has_value();
			}
		};

		public:
		using key_type =
			KeyType;	///< The type of the keys used in the cache.
		using mapped_type = CacheEntry;	   ///< The type of the values stored in
										   ///< the cache (CacheEntry).
		using container_type =
			std::map<KeyType,
					 CacheEntry>;	 ///< The underlying container
									 ///< type used for storing cache
									 ///< entries.
		using size_type =
			typename container_type::size_type;	   ///< The type used for
												   ///< representing the size of
												   ///< the cache.

		private:
		container_type _cache;	  ///< The underlying container that holds the
								  ///< cache entries.

		public:
		/**
		 * @brief Constructs a new Cache object.
		 *
		 * Initializes an empty cache system.
		 */
		Cache(void) = default;

		/**
		 * @brief Copy constructor for Cache.
		 *
		 * @param other The Cache object to copy from.
		 */
		Cache(const Cache &) = default;

		/**
		 * @brief Move constructor for Cache.
		 *
		 * @param other The Cache object to move from.
		 */
		Cache(Cache &&) noexcept = default;

		/**
		 * @brief Copy assignment operator for Cache.
		 *
		 * @param other The Cache object to copy from.
		 * @return A reference to the current Cache object.
		 */
		Cache &operator=(const Cache &) = default;

		/**
		 * @brief Move assignment operator for Cache.
		 *
		 * @param other The Cache object to move from.
		 * @return A reference to the current Cache object.
		 */
		Cache &operator=(Cache &&) noexcept = default;

		/**
		 * @brief Destructor for Cache.
		 *
		 * Cleans up any resources used by the cache system.
		 */
		virtual ~Cache(void) = default;

		/**
		 * @brief Checks if the cache contains an entry for the given key.
		 *
		 * @param key The key to check for in the cache.
		 * @return true if the cache contains an entry for the key, false
		 * otherwise.
		 */
		[[nodiscard]] bool contains(const KeyType &key) const
		{
			return _cache.contains(key);
		}

		/**
		 * @brief Checks if the cache entry for the given key is valid and
		 * matches the provided current value.
		 *
		 * @param key The key to check in the cache.
		 * @param currentValue The current value to compare against the cached
		 * value.
		 * @return true if the cache entry is valid and matches the current
		 * value, false otherwise.
		 */
		[[nodiscard]] bool isValid(const KeyType &key,
								   const ValueType &currentValue) const
			requires std::equality_comparable<ValueType>
		{
			const auto it = _cache.find(key);
			return it != _cache.end() && it->second.value.has_value()
				&& *it->second.value == currentValue;
		}

		/**
		 * @brief Finds the cache entry for the given key.
		 *
		 * @param key The key to find in the cache.
		 * @return A pointer to the CacheEntry if found, nullptr otherwise.
		 */
		[[nodiscard]] CacheEntry *findEntry(const KeyType &key) noexcept
		{
			const auto it = _cache.find(key);
			return it != _cache.end() ? &it->second : nullptr;
		}

		/**
		 * @brief Finds the cache entry for the given key (const version).
		 *
		 * @param key The key to find in the cache.
		 * @return A pointer to the CacheEntry if found, nullptr otherwise.
		 */
		[[nodiscard]] const CacheEntry *
			findEntry(const KeyType &key) const noexcept
		{
			const auto it = _cache.find(key);
			return it != _cache.end() ? &it->second : nullptr;
		}

		/**
		 * @brief Gets the cache entry for the given key, creating it if it
		 * doesn't exist.
		 *
		 * @param key The key to get or create in the cache.
		 * @return A reference to the CacheEntry.
		 */
		CacheEntry &getOrCreateEntry(const KeyType &key)
		{
			return _cache.try_emplace(key).first->second;
		}

		/**
		 * @brief Emplaces a value into the cache for the given key.
		 *
		 * If an entry for the key already exists, it will be updated with the
		 * new value. If no entry exists, a new one will be created.
		 *
		 * @tparam Args The types of the arguments to forward to the value's
		 * constructor.
		 * @param key The key for which to emplace the value.
		 * @param args The arguments to forward to the value's constructor.
		 * @return A reference to the CacheEntry containing the emplaced value.
		 */
		template<typename... Args>
		CacheEntry &emplaceValue(const KeyType &key, Args &&...args)
		{
			auto &entry = getOrCreateEntry(key);
			entry.value.emplace(std::forward<Args>(args)...);
			entry.used = true;
			return entry;
		}

		/**
		 * @brief Erases the cache entry for the given key.
		 *
		 * @param key The key of the entry to erase.
		 * @return true if an entry was erased, false if no entry existed for
		 * the key.
		 */
		bool erase(const KeyType &key)
		{
			return _cache.erase(key) > 0;
		}

		/**
		 * @brief Clears all entries from the cache.
		 *
		 * This function removes all key-value pairs from the cache, leaving it
		 * empty.
		 */
		void clear() noexcept
		{
			_cache.clear();
		}

		/**
		 * @brief Marks the cache entry for the given key as used.
		 *
		 * If an entry for the key exists, its 'used' flag will be set to true.
		 *
		 * @param key The key of the entry to mark as used.
		 */
		void markAsUsed(const KeyType &key)
		{
			if (auto *entry = findEntry(key)) {
				entry->used = true;
			}
		}

		/**
		 * @brief Marks the cache entry for the given key as unused.
		 *
		 * If an entry for the key exists, its 'used' flag will be set to false.
		 *
		 * @param key The key of the entry to mark as unused.
		 */
		void markAsUnused(const KeyType &key)
		{
			if (auto *entry = findEntry(key)) {
				entry->used = false;
			}
		}

		/**
		 * @brief Resets the 'used' flag for all cache entries to false.
		 *
		 * This function iterates through all entries in the cache and sets
		 * their 'used' flags to false, effectively marking them as unused.
		 */
		void resetUsage(void) noexcept
		{
			for (auto &[_, entry]: _cache) {
				entry.used = false;
			}
		}

		/**
		 * @brief Gets the number of entries in the cache.
		 *
		 * @return The number of key-value pairs currently stored in the cache.
		 */
		[[nodiscard]] size_type size(void) const noexcept
		{
			return _cache.size();
		}

		/**
		 * @brief Checks if the cache is empty.
		 *
		 * @return true if the cache contains no entries, false otherwise.
		 */
		[[nodiscard]] bool empty(void) const noexcept
		{
			return _cache.empty();
		}

		/**
		 * @brief Gets a list of keys for entries that are marked as unused.
		 *
		 * This function iterates through the cache and collects the keys of
		 * entries whose 'used' flag is false.
		 *
		 * @return A vector containing the keys of unused cache entries.
		 */
		[[nodiscard]] std::vector<KeyType> getUnusedKeys(void) const
		{
			std::vector<KeyType> unusedKeys;
			for (const auto &[key, entry]: _cache) {
				if (!entry.used) {
					unusedKeys.push_back(key);
				}
			}
			return unusedKeys;
		}
	};

}	 // namespace utility