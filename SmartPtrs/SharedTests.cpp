#include "SharedTests.h"

#include <vector>
#include <memory>

namespace smart_ptrs
{

    struct Person
    {
        using PersonShared = std::shared_ptr<Person>;
        using PersonKid = std::shared_ptr<Person>; // must be weak_ptr to succeed

        std::string name;
        PersonShared  mom;
        PersonShared  dad;

        std::vector<PersonKid> kids;

        Person(const std::string& n, PersonShared m = nullptr, PersonShared d = nullptr)
            : name(n), mom(m), dad(d)
        {
        }

        ~Person()
        {
            std::cout << "delete " << name << std::endl;
        }
    };


    Person::PersonShared initFamily(const std::string& name)
    {
        Person::PersonShared mom(new Person(name + "'s mom"));
        Person::PersonShared dad(new Person(name + "'s mom"));
        Person::PersonShared kid(new Person(name, mom, dad));

        mom->kids.push_back(kid);
        dad->kids.push_back(kid);
        return kid;
    }


    void test_shared_dangling()
    {
        Person::PersonShared p = initFamily("person");

        p = initFamily("test");
        std::cout << "test exists" << std::endl;
    }
}