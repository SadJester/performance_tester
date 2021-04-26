// StdTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "SmartPtrs/SharedTests.h"
#include "Containers/ListVsVector.h"
#include "Containers/UnorderedSetVsCustom.h"
#include "Miscellaneous/MoveSemantics.h"
#include "Miscellaneous/StringsTest.h"

#include <vector>
#include <string>
#include <string_view>



namespace T
{
	template <typename Key, typename Value>
	class MultipleValContainer
	{
	public:
		using _Pair = std::pair<Key, Value>;
		using _Container = std::vector<_Pair>;
		using iterator = typename _Container::iterator;
		using const_iterator = typename _Container::const_iterator;

	public:
		template <typename K, typename V,
			typename = std::enable_if<(std::is_same<K, Key>::value || std::is_base_of<Key, K>::value)
			&& (std::is_same<V, Value>::value || std::is_base_of<Value, V>::value)>
		>
			void emplace(K&& key, V&& val)
		{
			_container.emplace_back(std::forward<K>(key), std::forward<V>(val));
		}

		void insert(_Pair&& keyVal)
		{
			_container.push_back(std::move(keyVal));
		}

		void erase(iterator it)
		{
			if (it != _container.end())
				_container.erase(it);
		}

		void try_erase(const Key& key)
		{
			erase(find(key));
		}

		const_iterator find(const Key& key) const
		{
			auto it = std::find_if(_container.begin(), _container.end(), [&key](auto& keyVal)
			{
				return keyVal.first == key;
			});
			return it;
		}

		iterator find(const Key& key)
		{
			auto it = std::find_if(_container.begin(), _container.end(), [&key](auto& keyVal)
			{
				return keyVal.first == key;
			});
			return it;
		}

		template <typename _View>
		const_iterator find_view(_View&& view) const
		{
			auto it = std::find_if(_container.begin(), _container.end(), [&view](auto& keyVal)
			{
				return keyVal.first == view;
			});
			return it;
		}

		bool exists(const Key& key) const {
			return find(key) != _container.end();
		}

		iterator begin() {
			return _container.begin();
		}

		iterator end() {
			return _container.end();
		}

		const_iterator begin() const {
			return _container.begin();
		}

		const_iterator end() const {
			return _container.end();
		}

		const _Container& container() const {
			return _container;
		}

		size_t size() const {
			return _container.size();
		}

		bool empty() const {
			return _container.empty();
		}

		void clear() {
			_container.clear();
		}

		void reserve(size_t capacity) {
			_container.reserve(capacity);
		}

	private:
		_Container _container;
	};


	struct EntityId
	{
		struct view
		{
			std::string_view moduleName;
			std::string_view typeName;
			std::string_view id;
		};

		std::string moduleName;
		std::string typeName;
		std::string id;

		friend bool operator == (EntityId const& a, const view& searchView);
	};

	bool operator == (EntityId const& a, const EntityId::view& searchView)
	{
		return a.moduleName == searchView.moduleName;
	}
}


std::vector<std::string_view> Split(std::string_view val, char delim)
{
	if (val.empty())
		return {};

	std::vector<std::string_view> splited;

	size_t idx = 0;
	const size_t size = val.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (delim == val[i])
		{
			if (idx != i)
				splited.push_back(std::string_view{ &val[idx], i - idx });
			idx = i + 1;
		}
	}


	if (idx < size)
	{
		splited.push_back(std::string_view{ &val[idx], size - idx });
	}

	return splited;
}


int main()
{
	auto spl0 = Split(".1.2.3.", '.');
	auto spl = Split("1.2.3.", '.');
	auto spl1 = Split("1.25.3.4", '.');
	auto spl2 = Split("4", '.');
	auto spl3 = Split("45", '.');
	auto spl4 = Split("", '.');

	//miscellaneous::test_strings();
	//miscellaneous::test_move_semantics();
    
	//smart_ptrs::test_shared_dangling();
    containers::test_list_vs_vector();
	//containers::test_unordered_set_vs_custom();
}
