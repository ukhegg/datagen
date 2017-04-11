# Datagen
Library for generating random test data 

# Usage

## Simple usage for build-in types
```cpp
#include <datagen/random.hpp>
#include <iostream>
int main(int argc, char* argv[])
{
	std::cout << "The answer to the question of everything is:" << datagen::random<int>() << std::endl;
	getchar();

}
```

will print something like 
```
The answer to the question of everything is:-511889235
```
## Simple usage for structs(or classes that can be constructed using braces notation)
The same way a structure can be generated(or class with open members)
```cpp
#include <datagen/random.hpp>
#include <iostream>
#include <ostream>

struct dummy
{
	int val;

	friend std::ostream& operator<<(std::ostream& os, dummy const& obj) { return os << "{val: " << obj.val << '}'; }
};

int main(int argc, char* argv[])
{
	std::cout << "random dummy is " << datagen::random<dummy>();
	getchar();
}
```
will print
```
random dummy is {val: 1147928012}
```
## Usage with types constructed via ctor
#### Simple case(not nested)
Let's change dummy from the previous example:
```
struct dummy
{
	explicit dummy(int val) : val_(val){}

	friend std::ostream& operator<<(std::ostream& os, dummy const& obj) 
	{ return os << "{val_: " << obj.val_ << '}'; }
private:
	int val_;
};
```
datagen can handle this case too:
```
random dummy is {val_: 470559295}
```
#### Complicated case(nested types generation)
```cpp
#include <datagen/random.hpp>
#include <iostream>
#include <ostream>

struct dummy_member
{
	float a;
	int b;
	double c;

	friend std::ostream& operator<<(std::ostream& os, dummy_member const& obj){ ... }
};

struct dummy
{
	explicit dummy(dummy_member val) : val_(val) {}

	friend std::ostream& operator<<(std::ostream& os, dummy const& obj) 
	{ return os << "{val_: " << obj.val_ << '}'; }
private:
	dummy_member val_;
};

int main(int argc, char* argv[])
{
	std::cout << "random dummy is " << datagen::random<dummy>();
	getchar();
}
```
will produce
```
random dummy is {val_: {a: -9.65806e+23 b: -399852536 c: 7.99812e+26}}
```
## Overriding generation algorithm
To use custom generation algorithm for T, just provide specialization of `struct value_generate_algorithm`
from namespace datagen with a `T get_random(random_source_base&)` member function
```cpp
#include <datagen/random.hpp>
#include <iostream>
#include <ostream>

struct dummy{
	float a;
	int b;
	double c;

	friend std::ostream& operator<<(std::ostream& os, dummy const& obj){
		return os
			<< "{a: " << obj.a
			<< " b: " << obj.b
			<< " c: " << obj.c << '}';
	}
};

namespace datagen{
	template<> struct value_generation_algorithm<dummy> {
		dummy get_random(random_source_base& r_source) {
			dummy d;
			d.a = 0.0;
			d.b = 42;
			d.c = r_source.create<decltype(d.c)>();
			return d;
		}
	};
}
int main(int argc, char* argv[]) {
	std::cout << "random dummy is " << datagen::random<dummy>();
	getchar();
}
//prints random dummy is {a: 0 b: 42 c: -5.28012e+147}
```

# Type construction 

When constructing a type, datagen looks for possibilities in the following order:
1. specialization of `struct value_generation_algorithm` with `get_random` method, something like this
```cpp
    template<class Ty, size_t N>
    struct value_generation_algorithm<std::array<Ty, N>> {
        using array_t = std::array<Ty, N>;

        array_t get_random(random_source_base &r_source) { ... }
    };
``` 
2. Possibility to construct type via {}-notation with max parameters
3. Type's public constructor with max possible parameter

# Type limits
Datagen provides a way to change explicit generation algorithm behavior, 
using algorithm and type limits. 
Simple example:
```cpp
#include <datagen/random.hpp>
#include <datagen/limits/builtin_limits.hpp>
#include <iostream>
#include <ostream>

using namespace datagen;
using namespace limits;

int main(int argc, char* argv[]) {
	std::cout << "The answer to the question of everything is:" << random<int>(between(42,42)) << std::endl;
	getchar();

}
// prints 'The answer to the question of everything is:42'
```
Limitations are separated in two categories: 
* **Algorithm limitations**

  Limitations of this kind are applied to `value_generation_algorithm<T>` before values of type `T` are generated. 
  So, you'll need custom `value_generation_algorithm<T>` to create useful algorithm limitations.
* **Value limitations**

  are applied to generated value.
  
  
Value generation algorithm looks something like(*pseudocode*):
```cpp
template<T> random(TLimits ... limits){
    random_source r_source;
    value_generation_algorithm<T> alg;
    apply_algorithm_limits(r_source, alg, limits);
    auto val = alg.get_random(r_source);
    apply_value_limits(r_source, val, limits);
    return val;
}
```
##### Custom limitations
To add custom `dummy_limit` for `struct dummy`, you need to implement the following free function:
```cpp
namespace datagen{
    namespace limits{
        //this method is responsible for algorithm limitation
        void adjust_algorithm(random_source_base&, 
                              dummy_limit const& limit, 
                              value_generation_algorithm<dummy>& alg){...}
        //this one is responsible for value limitations   
        void adjust_value(random_source_base&, 
                          dummy_limit const& limit, 
                          dummy& value){...}
    }
}
```
See an example:
```cpp
struct dummy{
	std::string greetings;
};

namespace datagen
{
	template<> struct value_generation_algorithm<dummy>{
		dummy get_random(random_source_base&){
			return dummy{ default_greetings };
		}
		std::string default_greetings{ "**** off!" };
	};
}
struct dummy_alg_limit{};
struct dummy_value_limit{};

namespace datagen{
	namespace limits{
		void adjust_algorithm(random_source_base&,
		                      dummy_alg_limit const& limit,
		                      value_generation_algorithm<dummy>& alg){
			alg.default_greetings = "Hello world";
		}

		void adjust_value(random_source_base&,
		                  dummy_value_limit const& limit,
		                  dummy& value){
			value.greetings.append("!");
		}
	}
}

int main(int argc, char* argv[])
{
	//prints '**** off!'
	std::cout << datagen::random<dummy>().greetings << std::endl;
	//prints 'Hello world!'
	std::cout << datagen::random<dummy>(dummy_alg_limit(), dummy_value_limit()).greetings;
}
```