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
## Simple usage for structs(or classes that can be constrcuted using braces notation)
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
To use custom generation algorithm, just provde specialization of struct value_generate_algorithm
from namespace datagen
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
1. specialization of class value_generate_algorithm with get_random method, something like this
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
using algorithm and type limits. Look:
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
Limitations are separated in two categories: algorithm limitations and value limitations.
Algorithm limitations are applyed to explicit generation algorithm before it is used.
Value limitations are applied to already generated value to fix it's value.
Both are implemented in single struct random_limit<T>
To create suctom algorithm/value limitation, derive from random_limit<T>
and override adjust_[algorithm|value] method:
```cpp
struct dummy {
	int answer1;
	int answer2;

	friend std::ostream& operator<<(std::ostream& os, dummy const& obj){
		return os << "{answer1:" << obj.answer1 << ",answer2:" << obj.answer2 << '}';
	}
};

namespace datagen {
	template<>
	struct value_generation_algorithm<dummy> {
		dummy get_random(random_source_base& r_source) { return dummy_prototype; }
		dummy dummy_prototype{ 42, 42 };
	};
}

struct dummy_alg_limit : public datagen::limits::random_limit<dummy>{
	~dummy_alg_limit() override = default;

	void adjust_value(datagen::random_source_base&, dummy& value) const override {
		value.answer2 = 41;
	}

	void adjust_algorithm(datagen::random_source_base&, datagen::value_generation_algorithm<dummy>& alg_params) const override {
		alg_params.dummy_prototype.answer1 = 43;
	}
};

int main(int argc, char* argv[]){
	std::cout << "random dummy is " << datagen::random<dummy>() << std::endl;
	std::cout << "random dummy is " << datagen::random<dummy>(dummy_alg_limit());
	getchar();
}
//prints:
//random dummy is {answer1:42,answer2:42}
//random dummy is {answer1:43,answer2:41}
```