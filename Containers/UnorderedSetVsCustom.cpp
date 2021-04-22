#include "UnorderedSetVsCustom.h"
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <array>

#include "../utilities/test_utilities.h"

struct Struct
{
	std::string x;
	std::string y;

	Struct() = default;
	Struct(std::string x_, std::string y_)
		: x(x_)
		, y(y_)
	{
		_hash = __get_hash(*this);
	}

	static size_t __get_hash(const Struct& s)
	{
		auto hash = std::hash<std::string>();
		return hash(s.x) ^ hash(s.y);
	}

	size_t _hash;
};



bool operator == (const Struct& l, const Struct& r)
{
	return l.x == r.x && l.y == r.y;
}

bool operator != (const Struct& l, const Struct& r)
{
	return l.x != r.x || l.y != r.y;
}

bool operator < (const Struct& l, const Struct& r)
{
	return l._hash < r._hash;
}

namespace std
{
	template<>
	struct hash<Struct>
	{
		size_t operator()(const Struct& source) const
		{
			return Struct::__get_hash(source);
		}
	};
}


namespace test_utilities {
	namespace generators
	{
		template <>
		struct Generator <Struct>
		{
			static Struct random()
			{
				return Struct{ Generator<std::string>::random(), Generator<std::string>::random() };
			}
		};
	}
}


namespace containers
{
	template <typename T>
	class UniqueElementsContainer
	{
	public:
		using _T = T;
		using _Container = std::vector<T>;
		using iterator = typename _Container::iterator;
		using const_iterator = typename _Container::const_iterator;

	public:
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

		template <typename _View>
		const_iterator find(_View&& search) const
		{
			auto itEnd = _container.end();
			auto it = std::lower_bound(_container.begin(), itEnd, search);
			if (it != itEnd && *it == search)
				return it;
			return itEnd;
		}

		template <typename _View>
		iterator find(_View&& search)
		{
			auto itEnd = _container.end();
			auto it = std::lower_bound(_container.begin(), itEnd, search);
			if (it != itEnd && *it == search)
				return it;
			return itEnd;
		}

		void erase(iterator it)
		{
			if (it != _container.end())
			{
				_container.erase(it);
			}
		}

		void try_erase(const T& elem)
		{
			erase(find(elem));
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

		template <typename V,
			typename = std::enable_if<std::is_same<V, _T>::value || std::is_base_of<_T, V>::value>
		>
			void insert(V&& val)
		{
			auto itEnd = _container.end();
			auto it = std::lower_bound(_container.begin(), _container.end(), val);

			if (it == itEnd || *it != val)
				_container.insert(it, std::move(val));
		}

	private:
		_Container _container;
	};


	template <typename Key, typename Value>
	class MultipleValContainer
	{
	public:
		using _Pair = std::pair<Key, Value>;
		using _Container = std::vector<_Pair>;
		using iterator = typename _Container::iterator;
		using const_iterator = typename _Container::const_iterator;

	public:
		template <typename K, typename V>
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


	struct SharedStruct
	{
		std::string name;
		std::string name_;
		SharedStruct(std::string_view n)
			: name(n)
			, name_(n)
		{
		}
	};

	using Links = std::unordered_map<Struct, std::shared_ptr<SharedStruct>>;

	template <typename Con>
	void test_container_key(const typename test_utilities::TestInfo<Struct>& info)
	{
		Con container;
		auto& _raw = info.dataOnStack;
		for (size_t i = 0; i < info.dataOnStack.size(); ++i)
		{
			container.emplace(_raw[i], std::make_shared<SharedStruct>(_raw[i].x));
		}
		container.clear();
	}

	template <typename Con>
	void test_container_key_(const test_utilities::TestInfo<Struct>& info)
	{
		Con container;
		auto& _raw = info.dataOnStack;
		for (size_t i = 0; i < info.number; ++i)
		{
			container.insert(std::pair(_raw[i], std::make_shared<SharedStruct>(_raw[i].x)));
		}
		container.clear();
	}

	template <typename Con>
	void test_container(const test_utilities::TestInfo<Struct>& info)
	{
		Con container;
		auto& _raw = info.dataOnStack;
		for (size_t i = 0; i < info.number; ++i)
		{
			container.insert(_raw[i]);
		}
		container.clear();
	}

	namespace TTT
	{
		template <typename T>
		bool IsEqual(const T& a, const T& b)
		{
			return a == b;
		}

		template<typename Iter>
		Iter HasUnique(Iter begin, Iter end, std::forward_iterator_tag)
		{
			for (auto it1 = begin; it1 != end; ++it1) {
				for (auto it2 = it1 + 1; it2 != end; ++it2) {
					if (IsEqual(*it1, *it2)) {
						return it2;
					}
				}
			}
			return end;
		}
		template<typename Iter>
		Iter HasUnique(Iter begin, Iter end)
		{
			return HasUnique(begin, end, typename std::iterator_traits<Iter>::iterator_category());
		}


		/////////////////////////////

		template <typename T,
			typename std::enable_if_t<!std::is_arithmetic_v<T>, bool> = true>
			bool insert_val(std::vector<const T*>& out, const T& val)
		{
			auto itEnd = out.end();
			const T* valPtr = &val;
			auto it = std::lower_bound(out.begin(), out.end(), valPtr, [](const T* lhs, const T* rhs) {
				return *lhs < *rhs;
			});

			if (it == itEnd || !(**it == val)) {
				out.insert(it, valPtr);
				return true;
			}

			return false;
		}

		template <typename T,
			typename std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
			bool insert_val(std::vector<T>& out, T val)
		{
			auto itEnd = out.end();
			auto it = std::lower_bound(out.begin(), out.end(), val);

			if (it == itEnd || !(*it == val)) {
				out.insert(it, val);
				return true;
			}

			return false;
		}


		template <typename T,
			typename std::enable_if_t<!std::is_arithmetic_v<T>, bool> = true>
		const T* GetDuplicateValue(const std::vector<T>& values)
		{
			std::vector<const T*> newValues;
			for (auto&& i : values) {
				bool res = insert_val(newValues, i);
				if (!res)
				{
					return &values[0];
				}
			}
			return nullptr;
		}


		template <typename T, 
			typename std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
		const T* GetDuplicateValue(const std::vector<T>& values)
		{
			std::vector<T> newValues;
			for (auto&& i : values) {
				bool res = insert_val<T>(newValues, i);
				if (!res)
				{
					return &values[0];
				}
			}
			return nullptr;
		}

		template <typename T>
		bool HasUnique_New(const std::vector<T>& values)
		{
			auto begin = values.begin();
			auto end = values.end();
			const T* duplicate = GetDuplicateValue<T>(values);
			auto it = std::find_if(begin, end, [duplicate](auto& i)
			{
				return *duplicate == i;
			});
			it = std::find_if(it + 1, end, [duplicate](auto& i)
			{
				return *duplicate == i;
			});
			size_t idx = std::distance(begin, it);
			return duplicate != nullptr;
		}

	}


	std::vector<std::string> create_data()
	{
		return {
			"ABC",
			"ABCD",
			"ABCDEFGHIJKLMNOPQRSTUVDAARQWER",
			"ASDQWfsadweravdfg",
			"123gatwt",
			"ABC",
			"asd134rafgdg0sdgf4kl5jawser",
			"90-dfg0ihl;fbkhs097-023452345",
			"asdf-035arasg09234aer",
			"asda08bsd09j24;k5j;lkfu0a9sdu8f1345049"
		};
	}


	void test_unordered_set_vs_custom()
	{
		using namespace test_utilities;
		constexpr int count = 5000;



		/*{
			auto t = Timer("HasUnique");
			for (int i = 0; i < count; ++i)
			{
				auto data = create_data();
				TTT::HasUnique(data.begin(), data.end());
			}
		}

		{
			auto t = Timer("HasUnique_Sort");
			for (int i = 0; i < count; ++i)
			{
				auto data = create_data();
				//TTT::HasUnique_New(std::vector<int>{1, 2, 3, 4, 1});
				TTT::HasUnique_New(data);
			}
		}*/


		const size_t cycles = 50;
		const std::vector<size_t> values{ 10, 50, 100, 1000 };
		//Tester::RunWithData<Struct>(&test_container<UniqueElementsContainer<Struct>>, { "custom", cycles, values });
		//Tester::RunWithData<Struct>(&test_container<std::unordered_set<Struct>>, { "unordered set", cycles, values });


		//Tester::RunWithData<Struct>(&test_container_key<MultipleValContainer<Struct, std::shared_ptr<SharedStruct>>>, { "custom", cycles, values });
		//Tester::RunWithData<Struct>(&test_container_key<std::unordered_map<Struct, std::shared_ptr<SharedStruct>>>, { "unordered map", cycles, values });


		Tester::Compare<Struct>({
			{"custom", &test_container_key<MultipleValContainer<Struct, std::shared_ptr<SharedStruct>>>},
			{"unordered_map", &test_container_key<std::unordered_map<Struct, std::shared_ptr<SharedStruct>>>} 
								},
								{ "Comparison", cycles, values });
	}

} // containers
