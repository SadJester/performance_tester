#include "MoveSemantics.h"
#include <vector>
#include <string>
#include <iostream>


namespace miscellaneous
{

	namespace move_semantics
	{
		struct Key
		{
			std::string val;
			Key(const char* name)
				: val(name)
			{
				std::cout << "Constructor for: " << val << std::endl;
			}

			Key(Key&& other) noexcept
				: val(std::move(other.val))
			{
				std::cout << "Move constructor for: " << val << std::endl;
			}

			Key(const Key& other)
				: val(other.val)
			{
				std::cout << "Copy constructor for: " << val << std::endl;
			}

			Key& operator = (const Key& other)
			{
				val = other.val;
				std::cout << "Copy operator for: " << val << std::endl;
				return *this;
			}

			Key& operator = (Key&& other) noexcept
			{
				val = other.val;
				std::cout << "Move operator: " << val << std::endl;
				return *this;
			}
		};

		Key getKey()
		{
			return Key{ "functionKey" };
		}

		std::ostream& operator << (std::ostream& s, const Key& v)
		{
			s << v.val;
			return s;
		}

		struct MyObj
		{
			Key _name;

			const Key& name() const
			{
				return _name;
			}
		};

		template <typename T>
		class Cont
		{
			std::vector<T> _values;
		public:
			Cont()
			{
				_values.reserve(100);
			}

			//void insert(const T& val)
			//{
			//	std::cout << ">" << "insert const&: " << val << std::endl;
			//	_values.push_back(val);
			//}
			//
			//void insert(T&& val)
			//{
			//	std::cout << ">" << "insert &&: " << val << std::endl;
			//	_values.push_back(std::move(val));
			//}

			std::vector<T> getValues()&& {
				std::cout << "move vals" << std::endl;
				return std::move(_values);
			}
			
			const std::vector<T>& getValues() const& {
				std::cout << "Const vals" << std::endl;
				return _values;
			}

			//const std::vector<T>& getValues() {
			//	std::cout << "Const vals" << std::endl;
			//	return _values;
			//}

			// If move operator is cheap; will call +1 move constructor for prvalue
			//void insert(T val)
			//{
			//	std::cout << ">" << "insert: " << val << std::endl;
			//	_values.push_back(std::move(val));
			//}

			template <typename U, 
				typename = std::enable_if<std::is_same<T, U>::value>
			>
			void insert(U&& val)
			{
				std::cout << ">" << "insert forward: " << val << std::endl;
				_values.push_back(std::forward<U>(val));
			}
		};

		Cont<std::string> getTestCont()
		{
			Cont<std::string> cont;
			cont.insert("One");
			cont.insert("Two");
			cont.insert("Three");
			return cont;
		}
	}

	void test_move_semantics()
	{
		namespace ms = move_semantics;
		ms::Cont<ms::Key> cont;
		ms::MyObj test{ "Name" };
		cont.insert(test.name());
		std::cout << std::endl;

		ms::Key separate{ "Separate" };
		cont.insert(std::move(separate));
		std::cout << std::endl;

		cont.insert("Simple");
		std::cout << std::endl;

		cont.insert(ms::getKey());

		std::cout << std::endl;

		auto t = ms::getTestCont();
		for (auto& t : t.getValues())
			std::cout << "> " << t << std::endl;

		for (auto& t : ms::getTestCont().getValues())
			std::cout << "> " << t << std::endl;
	}

}