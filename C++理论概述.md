# C++

## C++引入

实现了 hdc 名称空间下的 list 模板类，目的是为了仿照 std 名称空间 list 模板类而设计的。其中包括的知识点有：类型重定义、模板函数、模板类、静态成员方法、常方法、 运算符重载、引用、继承以及迭代器。

```c++
namespace hdc
{
	template<class T>
	void swap(T& a, T& b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}
	template<class _Ty>
	class list
	{
	private:
		struct _Node;
		typedef struct _Node* _Nodeptr;// 代表指向结点的指针
		struct _Node
		{
			_Nodeptr _Prev, _Next;
			_Ty _Value;
		};
		struct _Acc;
		struct _Acc
		{
			typedef struct _Node*& _Nodepref;// 代表结点指针的引用
			typedef _Ty& _Vref;// 代表数据类型的引用
			static _Vref _Value(_Nodeptr _P)
			{
				return (*_P)._Value;
			}
			static _Nodepref _Prev(_Nodeptr _P)
			{
				return (*_P)._Prev;
			}
			static _Nodepref _Next(_Nodeptr _P)
			{
				return (*_P)._Next; // return _P->_Next;
			}
		};
	public:
		typedef _Ty          value_type;// 数据类型
		typedef _Ty&         reference;// 数据类型的引用
		typedef const _Ty&   const_reference;// 数据类型的常引用
		typedef _Ty*         pointer;// 数据类型的指针
		typedef const _Ty*   const_pointer;// 数据类型的常指针
		typedef size_t       size_type;// 数据长度
		typedef int          difference_type;

	public:

		class const_iterator
		{
		protected:
			_Nodeptr _Ptr;// 结点类型的指针
		public:
			const_iterator(_Nodeptr _P = NULL) :_Ptr(_P) {}
			const_reference operator*() const
			{
				return _Acc::_Value(_Ptr);// 返回结点中的数据域
			}
			const_pointer operator->() const
			{
				return &**this;// *this代表当前迭代器，然后调动*运算符重载返回结点中的数据域，&取数据域的地址
			}
			const_iterator& operator++()
			{
				_Ptr = _Acc::_Next(_Ptr);
				return *this;
			}
			const_iterator operator++(int)
			{
				const_iterator tmp = *this;
				++* this;
				return tmp;
			}
			const_iterator& operator--()
			{
				_Ptr = _Acc::_Prev(_Ptr);
				return *this;
			}
			const_iterator operator--(int)
			{
				const_iterator tmp = *this;
				--* this;
				return tmp;
			}
			bool operator==(const const_iterator& _X) const
			{
				return this->_Ptr == _X._Ptr;
			}
			bool operator!=(const const_iterator& _X) const
			{
				return !(*this == _X);
			}
			_Nodeptr _Mynode() const
			{
				return _Ptr;// 返回当前迭代器中的结点指针
			}
		};
		class iterator : public const_iterator// 普通迭代器公有继承常性迭代器
		{
			typedef const_iterator base;
		public:
			iterator(_Nodeptr _P = NULL) :const_iterator(_P) {}
			reference operator*() const
			{
				return _Acc::_Value(base::_Ptr);
			}
			pointer operator->() const
			{
				return &**this;
			}
			iterator& operator++()
			{
				base::_Ptr = _Acc::_Next(base::_Ptr);
				return *this;
			}
			iterator operator++(int)
			{
				iterator tmp = *this;
				++* this;
				return tmp;
			}
			iterator& operator--()
			{
				base::_Ptr = _Acc::_Prev(base::_Ptr);
				return *this;
			}
			iterator operator--(int)
			{
				iterator tmp = *this;
				--* this;
				return tmp;
			}
			bool operator==(const iterator& _X) const
			{
				return this->_Ptr == _X._Ptr;
			}
			bool operator !=(const iterator& _X) const
			{
				return !(*this == _X);
			}
		};

		iterator begin() 
         {
			return iterator(_Acc::_Next(_Head));// 返回指向链表头结点的迭代器
		}
		iterator end() 
         {
			return iterator(_Head);// 返还指向链表最后一个有效结点的下一个结点，双向循环链表即为头结点
		}
		const_iterator begin() const
		{
			return const_iterator(_Acc::_Next(_Head));
		}
		const_iterator end() const
		{
			return const_iterator(_Head);
		}

	public:
		typedef const_iterator _It;
		list() :_Head(_Buynode()), _Size(0) {}// 默认构造函数，只有头结点，长度为零
		list(std::initializer_list<_Ty> list) :_Head(_Buynode()), _Size(0)// 初始化列表方式的构造函数
		{
			for (auto& x : list)
			{
				push_back(x);
			}
		}
		list(size_t count, const _Ty& val) :_Head(_Buynode()), _Size(0)// 初始化count个值为val的构造函数
		{
			insert(begin(), count, val);
		}
		list(const _Ty* _F, const _Ty* _L) :_Head(_Buynode()), _Size(0)// 采用数组方式的构造函数
		{
			insert(begin(), _F, _L);
		}
		list(const list& _X) :_Head(_Buynode()), _Size(0)// 拷贝构造函数
		{
			insert(begin(), _X.begin(), _X.end());
		}
		list& operator=(const list& _X)// 赋值运算符重载
		{
			if (this == &_X)return *this;
			iterator _F1 = begin();
			iterator _L1 = end();
			const_iterator _F2 = _X.begin();
			const_iterator _L2 = _X.end();

			for (;_F1 != _L1 && _F2 != _L2;++_F1, ++_F2)
			{
				*_F1 = *_F2;
			}
			erase(_F1, _L1);
			insert(_L1, _F2, _L2);
			return *this;
		}
		~list()// 析构函数
		{
			clear();
			_Freenode(_Head);
		}
		void push_front(const _Ty& val)
		{
			insert(begin(), val);
		}
		void push_back(const _Ty& val)
		{
			insert(end(), val);
		}
		void insert(iterator _P, const _Ty* _F, const _Ty* _L)// 将数组中_F到_L的所有值在_P位置插入结点
		{
			for (;_F != _L;++_F)
			{
				insert(_P, *_F);
			}
		}
		void insert(iterator _P, size_t count, const _Ty& val)// 将count个值为val的结点插入_P位置
		{
			while (count--)
			{
				insert(_P, val);
			}
		}
		void insert(iterator _P, _It _F, _It _L)// 将迭代器_F到_L的所有结点插入_P位置
		{
			for (;_F != _L;++_F)
			{
				insert(_P, *_F);
			}
		}
		void insert(iterator _P, std::initializer_list<_Ty> list)// 初始化列表方式在_P位置插入结点
		{
			for (auto& x : list)
			{
				insert(_P, x);
			}
		}
		iterator insert(iterator _P, const _Ty& val) // 将一个值为val的结点插入_P位置
		{
			_Nodeptr _S = _P._Mynode();
			_Acc::_Prev(_S) = _Buynode(_Acc::_Prev(_S), _S);// 构建新的结点
			_S = _Acc::_Prev(_S);
			_Acc::_Next(_Acc::_Prev(_S)) = _S;
			new(&_Acc::_Value(_S))_Ty(val);// 调用结点数据域的构造函数
			this->_Size += 1;
			return iterator(_S);// 返回新构建结点的迭代器
		}
		void pop_front()
		{
			erase(begin());
		}
		void pop_back()
		{
			erase(--end());
		}
		void erase(iterator _F, iterator _L)// 将迭代器_F到_L的所有结点删除
		{
			while (_F != _L)
			{
				erase(_F++);
			}
		}
		void clear()// 清空链表所有结点
		{
			erase(begin(), end());
		}
		void remove(const _Ty& val)// 删除第一个值为val的结点
		{
			iterator _F = begin(), _L = end();
			while (_F != _L)
			{
				if (*_F == val)
				{
					erase(_F);
					break;
				}
				++_F;
			}
		}
		void remove_all(const _Ty& val)// 删除链表中所有值为val的结点
		{
			iterator _F = begin(), _L = end();
			while (_F != _L)
			{
				if (*_F == val)
				{
					erase(_F++);
				}
				else
				{
					++_F;
				}
			}
		}
		iterator erase(iterator _P)// 删除_P位置的结点
		{
			_Nodeptr _S = _P++._Mynode();
			_Acc::_Next(_Acc::_Prev(_S)) = _Acc::_Next(_S);
			_Acc::_Prev(_Acc::_Next(_S)) = _Acc::_Prev(_S);
			(&_Acc::_Value(_S))->~_Ty();
			_Freenode(_S);
			this->_Size -= 1;
			return _P;
		}
		void swap(list& _X)// 交换两个链表的内容
		{
			hdc::swap(this->_Head, _X._Head);
			hdc::swap(this->_Size, _X._Size);
		}
	private:
		_Nodeptr _Buynode(_Nodeptr _Parg = NULL, _Nodeptr _Narg = NULL)// 构建新结点
		{
			_Nodeptr _S = (_Nodeptr)malloc(sizeof(_Node));
			_Acc::_Prev(_S) = _Parg == NULL ? _S : _Parg;
			_Acc::_Next(_S) = _Narg == NULL ? _S : _Narg;
			return _S;
		}
		void _Freenode(_Nodeptr _P)// 释放结点
		{
			free(_P);
		}
		_Nodeptr _Head;// 头结点
		size_type _Size;// 链表长度
	};
}
```

## C++基本知识

### 函数默认值

允许函数参数带默认值。**给形参默认值时必须从右向左依次赋值，不能跳过**。形参默认值作用范围只在本文件，因为默认值需要在编译时期入参。使用函数默认值的时候，需要注意两点：函数在调用的时候可以获得设置的默认参数；默认值不存在重复设置。一般来讲是在函数声明的时候将默认值给出，因为函数的声明对于用户是可见的。

### inline与内联函数

inline 关键字只是对系统的建议 --- 建议生成内联，具体是否真的将其变成内联函数由编译器决定。一般来说，内联函数的函数体不超过五行，并且执行功能单一，不包含循环或者选择语句。需要注意的是，当环境处于 debug 版本时，不会产生内联，即便用户加上inline关键字；**只有当环境处于 release 版本时，才有可能产生内联**。内联函数的作用是提高程序运行的效率，如果一个函数是内联的，那么在编译时，编译器会把该函数的代码副本放置在每个调用该函数的地方。因为内联函数会在调用处直接展开，不会执行正常的函数调用过程，是一种空间换时间的策略，所以内联函数一般要是函数体较短的函数。

### 函数重载

在C++中可以对函数和运算符进行重载。在同一个作用域内，可以声明几个功能类似的同名函数，但是这些同名函数的形式参数（指参数的个数、类型或者顺序）必须不同，返回值不同不能作为函数重载的标志。还可以重定义或重载大部分 C++ 内置的运算符。这样，用户就能使用自定义类型的运算符。

C++可以实现函数重载的根本原因在于其对函数的解析方式与C语言不同，C语言仅仅以函数名作为区分不同函数的标志，而C++以返回值、函数名、参数列表作为区分不同函数的标志。即便是这样，仅仅返回值不同的函数不能重载，因为这有可能产生二义性；在函数同时存在默认值和重载时，有可能产生二义性，具体需要视情况而定。

### const关键字

在C语言中的常量其实是常变量，可以通过一些间接的手段改变原先常量的值。在C++中，常量就是常量，**不允许通过任何方式改变常量的值**（比如说不允许泄漏常量的地址给非常量的指针）。这是因为在编译期，常量的值直接通过立即数的方式写入到常量的使用点。常量要用常量初始化，如果使用变量给常量初始化，则这个常量会退化成常变量。

常方法的`const`关键字修饰了`this`指针，使其不能修改成员变量。普通对象可以调用普通方法和常方法，常对象只能调用常方法（构造函数，析构函数，静态函数不影响）；普通方法可以调用普通方法和常方法，常方法中只能调用常方法（静态函数不影响）。成员方法需要写成常方法的几种情况：如果成员方法内不改动成员，并且没有对外暴露成员引用或者指针，可以直接写成常方法；如果成员方法内不改动成员，但是会对外界暴露成员引用或者指针，可以写两个成员方法形成重载；如果成员方法内部需要改动成员，就写成普通方法。

```c++
//const和指针
int a = 10, b = 20;
const int* s = &a;//不能改变s指向的值
int* p1 = s;//error，通过p1可以改变s指向的值
const int* p2 = s;//ok
int* const p3 = s;//error，通过p3可以改变s指向的值
const int* const p4 = s;//ok

int* const s = &a;//不能改变s的值
int* p1 = s;//ok，p1改变不影响s的值
const int* p2 = s;//ok，p2改变不影响s的值
int* const p3 = s;//ok，p3具有常性
const int* const p4 = s;//ok，p4具有常性
```

### 引用

引用的本质可以从两个方面来看：**语法层面上来看，引用是变量的别名；汇编层面上来看，引用相当于自身为常性的指针**。在使用到引用的地方，编译期会自动替换成底层指针的解引用。当函数返回一个引用时，指的就是返回该返回值的地址。需要注意的是：不允许返回临时变量的引用，或者说不要返回临时变量的地址。也就是说当变量的生存期不会被函数的生存期所影响，那么这个变量可以作为引用返回。静态变量，全局变量，通过引用传进来的变量可以通过引用返回。

```c++
//const和引用
int a = 10, b = 20;
const int* s = &a;//不能改变s指向的值
int*& p1 = s;//error，通过p1可以改变s指向的值
const int*& p2 = s;//ok
int* const& p3 = s;//error，通过p3可以改变s指向的值
const int* const& p4 = s;//ok

int* const s = &a;//不能改变s的值
int*& p1 = s;//error，p1是s的引用，p1的改变将会引起s的改变
const int*& p2 = s;//error，p2是s的引用，p2的改变将会引起s的改变
int* const& p3 = s;//ok，该引用具有常性
const int* const& p4 = s;//ok，该引用具有常性
```

### this指针

`this`指针指向当前调用方法的对象，本质是一个常量指针：可以修改指针指向的值，但不能修改指针的指向。`this`指针在编译时期被自动添加：普通成员方法的第一个参数，默认加上`this`指针；在普通成员方法内使用到普通成员的地方，加上`this->`；在调用成员方法的时候加上参数`this`指针。在调用对象的成员方法时，实参中默认加入调用对象的地址。

`this`指针优化了内存分配方案。没有`this`指针，每创建一个对象，系统都要为其成员属性和方法分配空间；而有了`this`指针之后，每个实例化的对象只需分配相应的属性空间，成员方法公有，通过`this`指针来判断是哪个对象调用了成员方法。

### 构造函数

当对象进行构造的时候默认调用的成员方法。如果没有实现构造函数，系统会生成一个默认构造函数也叫缺省构造函数。当自己实现了构造函数之后，系统就不会生成默认构造函数了。构造函数具有三个功能：构造对象、初始化对象、类型转换。类型转换只适用于一个参数的构造函数，也就是说会将用户传入的参数进行隐式转换，关键字 **explicit** 可以防止这种隐式转换。

### 析构函数

对象生存周期满了之后自动调用的成员方法。析构函数如果没有自己实现，系统会默认生成一个什么都不做的析构函数。一旦自己实现，就不会再生成默认的析构函数。

### 拷贝构造函数

用一个已存在的对象给另一个正在生成的对象初始化的时候自动调用的成员方法，如果没有自己实现，会自动生成一个浅拷贝的构造函数，如果自己实现，就不会生成默认的拷贝构造函数。拷贝构造函数分两种，一种接收对象的引用 -- 即普通的拷贝构造函数；另一种接收右值引用 -- 即**移动拷贝构造函数**，右值引用可以引用不可取地址的量，比如说函数返回值，临时构建的对象等。如果在需要调动拷贝构造函数情况下，编译器发现参数是一个右值引用类型，那么会优先调动移动拷贝构造函数，否则就调用普通构造函数。移动拷贝构造函数的根本作用是将资源进行转移，将临时对象中的资源转移给另一个对象，这样做之后临时对象本身不具有资源，而将资源给了当前调用拷贝构造函数的对象。这样做是有意义的，因为当前调用对象就是需要被构造的对象，而临时对象本身就是将亡值，无论如何都会在调用完成后释放，它是否拥有资源并不重要。

### 赋值运算符重载

当使用一个已存在的对象，给另一个已存在的对象赋值时候自动调用的成员方法。如果不自己实现，就会自动生成一个浅拷贝的赋值运算符重载函数。如果实现了，就不会自动生成赋值运算符重载。同样的，赋值运算符重载也有**移动赋值运算符重载**，接收右值引用类型，将临时对象的资源移动到当前调用对象中。

无论是移动拷贝构造函数，还是移动赋值运算符重载函数，都是将对象资源进行转移操作，一方面可以提高执行效率，另一方面是对资源的合理利用。有别于普通对象，临时对象使用完就被释放，转移一个即将被释放的对象中的资源不存在问题，所以使用移动函数；对于普通对象，即便是用来初始化或者赋值另一个对象，原先的资源也不应该被转移，所以使用普通函数。

### 初始化列表

只有构造函数可以使用初始化列表，其他函数不允许有初始化列表，因为对象只能被构建一次。必须初始化的成员需要放在初始化列表，在本对象构造之前需要完成的动作，必须放在初始化列表。C++中还有一种初始化列表类叫做 initializer_list，专门用以处理用花括号组织的元素集合。可以单独使用，也可以作为一个函数的形参，通过枚举方式获取列表中的元素。

### 静态成员变量

静态成员变量，一个类只存在一份，存储在数据段。必须在类外初始化（`.cpp`文件中初始化），且只能初始化一次。静态成员变量的访问可以不依赖于对象（不依赖于`this`指针），使用类的作用域可以直接访问。

### 静态成员方法

静态成员方法没有`this`指针，静态成员方法的访问可以不依赖于对象（不依赖于`this`指针），使用类的作用域可以直接访问。静态成员方法内只能使用静态成员，原因也是静态成员方法没有`this`指针。

### 组合与代理

组合指的是一个类是另一个类的一部分。嵌套类也属于组合的一种，指的是一个类包含另一个类。嵌套类拥有完整的权限，外界访问嵌套类需要用到外层类的作用域。成员对象指的是一个类中有另外一个类的对象，先构造成员对象再构造自身对象，析构顺序相反。如果成员对象没有默认的构造函数，必须要在初始化列表实现。

一个类的接口是另一个类的子集，或者说一个类的接口完全依赖于另一个类的接口功能就叫做代理。在C++引入中 stack 类、queue 类和 list 类的关系就是代理。

### 继承

基于一个类去定义另外一个类。派生类无法访问基类的私有成员，有以下三种继承方式：

- `public`派生类、自身类、外界都可以访问

- `protected`派生类和自身类可以访问

- `private`只有自身类可以访问

对于派生类来说，结构体的缺省继承为共有，类的缺省继承为私有，这与基类无关。构造派生类对象时首先要构建基类对象，在基类中应该提供合适的构造函数（无论是显示还是隐式）。在继承过程中，可以将派生类对象赋值给基类对象，可以将派生类对象的地址赋值给基类对象的指针，可以使用派生类对象初始化基类对象的引用，这就是**赋值兼容性规则**。派生类对象中含有基类的隐藏对象，一般会默认调用基类的无参构造函数，拷贝构造函数和赋值运算符重载函数需要显式调用。派生类会隐藏基类的同名成员（同名隐藏），访问被隐藏的成员需要加上（基类）作用域。

### 运行时多态

也叫做动多态、晚绑定、动态联编。使用指针或者引用调用虚函数，就会产生动多态调用，以下是动多态调用的过程：

- 使用指针或者引用调用虚函数
- 在对象中找到`vfptr`
- 根据`vfptr`找到`vftable`
- 在`vftable`中找到要调用的函数
- 调用函数

**虚函数表（`vftable`）**

虚函数表存储于`.rodata`段（只读数据段），编译时期如果发现类中有虚函数，就会对这个类生成虚函数表，将当前类中的虚函数指针写入虚函数表中。构建基类对象时，如果发现该类有虚函数表，那么产生一个指针指向虚函数表的首地址，即虚表指针。派生类对象继承基类对象后，会更改从基类继承来的虚表指针，使其重新指向自己的虚函数表。虚表指针存在于类对象中，而虚函数表每个类只有一份。

**覆盖**

覆盖发生在`vftable`中（也叫重写），父类中的虚函数会被子类中相同的函数覆盖（在子类中的虚函数表中覆盖）。

**类的编译顺序**

编译类名，编译成员名，编译嵌套类，编译成员方法体。

### 迭代器

迭代器可以理解为指向容器内部数据的一个指针，一般作为嵌套类实现，使用时将其实例化成对象。使用迭代器时需要注意其所指向的位置，尤其是在使用迭代器进行插入删除操作的时候，忽略返回值可能造成当前迭代器失效或者指向错误的位置。即便如此，使用STL容器提供的方法仍然可能造成迭代器丢失。

### 函数模板

函数模板不编译，不可以直接运行，函数模板会在编译期根据使用情况生成对应的函数。但是模板生成的函数指令会进行编译，模板中的语法错误会在生成对应的指令时候被找出并报错。使用函数模板可以不用传模板类型参数因为函数模板具有**类型推演**的能力。模板特例化的优先级比普通模板要高，普通函数的优先级比模板特例化要高。

### 类模板

类模板不编译，会在编译期根据使用方式，生成对应的类代码（指令）。类模板中没有使用到的成员方法不会在编译期生成对应的指令。类模板使用时候必须加上模板类型参数，因为类模板无法自己推导类型参数。类模板的成员方法只能在头文件中实现，或者在使用到的源文件中实现，不能分开实现。

**为什么类模板的成员方法不能在其他`.cpp`文件中实现？**

**类模板需要在编译时期将使用到的成员方法生成对应的指令，编译期是只针对单文件的，如果将类模板的成员方法实现在了其他文件，编译期使用到该方法的文件不可见，就无法生成对应的指令 --- “无法解析的外部符号”。**

### 抽象类

有纯虚函数的类叫做抽象类，抽象类无法实例化对象。抽象类的作用是要求派生类必须覆盖某一个接口，如果派生类不重写从基类继承来的纯虚函数，那么这个派生类也是抽象类。如果一个类（或者结构体）中只包含纯虚函数，这意味着以该类为基类的派生类必须重写所有的纯虚函数，此时这个基类称为**接口**。

## C++常见问题

**什么是多态？**

多态分为静多态和动多态。

静多态：编译时期的多态，又被称为早绑定。代表：函数重载。

动多态：运行时期的多态，又被称为晚绑定。代表：继承中的多态。

**什么是隐藏？**

派生类中的成员会隐藏基类中同名的成员。

**什么是覆盖？**

派生类中的成员会覆盖基类中相同的虚函数。

**动多态的产生条件？**

通过指针或者引用调用虚函数，而且对象必须完整。完整对象：构造函数执行完毕，析构函数还没开始。

**动多态的过程？**

通过指针或者引用调用虚函数，在对象中找到`vfptr`，通过`vfptr`找到`vftable`，在`vftable`中找到要调用的函数。

**`vftable`什么时候产生？在哪里存储？**

编译时期产生，存储在只读数据段。

**构造函数能不能写成虚函数？**

不能。构造函数无法通过指针或者引用调用，所以没有意义；`vfptr`是在构造时写入对象，而动态调用虚函数需要用到`vfptr`。

**静态函数能不能写成虚函数？**

不能。静态函数不依赖于对象，无法产生动多态。

**析构函数能不能写成虚函数？**

能。

**虚函数能不能处理成内联？**

不能。虚函数在编译期需要将函数指针放到`vftable`，而内联函数在编译期展开。

**什么情况下析构函数必须写成虚函数？**

当存在基类指针指向堆上的派生类对象的时候，就必须把基类的析构函数写成虚函数。

**父类指针指向子类对象的时候，指向子类中哪个位置？**

指向父类空间开始的地方。

**类的编译顺序？**

编译类名，编译成员名，编译嵌套类，编译成员方法体。

**父类指针能不能指向子类对象？子类指针能不能指向父类对象？**

能，不能。

**什么是RTTI，RTTI在什么时候产生？RTTI信息存储在哪里？**

运行时期的类型信息，是一个指向类型信息的指针。编译期产生，RTTI指针放在`vftable`里面，类型信息放在`.rodata`段当中，符号`dynamic_cast`是父类指针转为子类指针专用的类型强转。使用前提是必须有RTTI，并且父类指针指向的对象中的RTTI确实是子类的。

**菱形继承引入的的问题？**

造成公共基类在派生对象中存在多个实例。

**菱形继承的解决？**

通过虚继承方式解决。

**虚继承的逻辑是什么？**

对于虚继承后的派生类对象，会优先构造虚基类对象放在派生类对象空间的底部，然后继续构造直接继承的基类，每个直接基类中隐藏基类对象的位置存放一个指针，指针指向的并不是虚基类对象的地址，而是相对于虚基类对象的偏移量。通过这种方式，保证了虚继承关系中只保留了一份公共基类。

## C++运算符重载

C++中大部分运算符均可以重载，由于其特殊性，编译器可以识别出哪些是运算符重载函数。运算符重载可以说是C++的核心特性，我们定义的类中默认就包含赋值运算符重载函数。需要注意的是，重载之后的运算符我们可以像一般运算符一样直接使用，但实际上编译器是将用户重载的运算符当作函数处理，也就是说调用规则遵循的是函数的调用规则，而内置的运算符则遵循运算符的调用规则，使用时需要将重载的运算符看做一个函数。下面通过 complex 类来一些说明常见运算符重载：

```c++
class Complex
{
public:
	Complex(int a, int b)
	{
		_a = a;
		_b = b;
	}

	Complex operator+(const Complex& src)const
	{
		int a = _a + src._a;
		int b = _b + src._b;
		return Complex(a, b);
	}

	Complex operator-(const Complex& src)const
	{
		int a = _a - src._a;
		int b = _b - src._b;
		return Complex(a, b);
	}

	bool operator &&(const Complex& src)const
	{
		return _a && src._a;
	}

	bool operator ||(const Complex& src)const
	{
		return _a || src._a;
	}

	Complex& operator ++()
	{
		++_a;
		return *this;
	}
    
    Complex operator ++(int)
	{
		int tmp = _a;
		_a++;
		return Complex(tmp, _b);
	}

	Complex& operator --()
	{
		--_a;
		return *this;
	}

	Complex operator --(int)
	{
		int tmp = _a;
		_a--;
		return Complex(tmp, _b);
	}
	friend istream& operator >>(istream& cin, Complex& src);
	friend ostream& operator <<(ostream& out, const Complex& src);

private:
	int _a;//实部
	int _b;//虚部
};

ostream& operator <<(ostream& cout, const Complex& src)
{
	cout << src._a << "+" << src._b << "i" << endl;
	return cout;
}

istream& operator >>(istream& cin, Complex& src)
{
	cout << "_a _b:";
	cin >> src._a >> src._b;
	return cin;
}
```

在某些情况下，需要进行特殊的运算符重载：强制类型转换运算符重载，一般用于隐式转换场景，比如将系统内置类型的变量直接赋值给类对象实例的情况；括号运算符重载，重载了括号运算符的函数一般称为仿函数，意味着可以像调用函数一样使用对象实例；布尔运算符重载，这是一种较为特殊的函数，没有返回值，没有参数列表，一般用于对象实例的条件判断中，比如判断对象内部指针是否为空等。

## C++模板

模板是C++泛型编程的基础，泛型编程指的是独立于任何特定的类型之外的方式编程。模板函数显著提高代码复用性，让程序看起来更加简洁；C++ STL 库中大量使用模板类，其功能之全面几乎可以适应各种使用环境。我们可以编写模板函数，或者实现模板类，在这其中，使用模板特例化来处理特殊情况。

### 函数模板

使用函数模板不需要用户特殊声明，只需要传入想要传入的参数即可，编译器会根据类型自推功能识别用户传入参数的类型，根据类型在编译期产生相应的函数。具体来说，模板类型参数可以完全特例化，部分特例化，不特例化。

下面是简单的模板函数使用例。模板类型参数 T 可以根据用户传入的参数进行类型自推，然后生成对应类型的函数。通过模板实现的排序函数不能处理字符串排序的情况，所以应该针对传入参数是字符串的情况来进行模板函数特例化。

```c++
/*交换函数模板*/
template<typename T>
void swap(T* a, T* b)
{
	T temp = *a;
	*a = *b;
	*b = temp;
}
/*排序函数模板*/
template<typename T>
void sort(T* arr, int len)
{
	for (int i = 0;i < len - 1;i++)
	{
		for (int j = 0;j < len - i - 1;j++)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}
/*排序函数模板特例化*/
template<>
void sort(const char** arr, int len)
{
	for (int i = 0;i < len - 1;i++)
	{
		for (int j = 0;j < len - i - 1;j++)
		{
			if (strcmp(arr[j], arr[j + 1]) > 0)
			{
				swap(&arr[j], &arr[j + 1]);
			}
		}
	}
}
```

### 类模板

下面介绍类模板的使用例。一般来说，编写类模板都要考虑到多种数据类型的存储情况，所以会给出部分特例化版本，在某些情况下还会给出完全特例化版本。类模板和函数模板很相似，但是不完全一样。函数模板可以进行类型自推而类模板不行，也就是说在使用类模板产生的对象时需要用户手动给出类型，通过尖括号给出。

```c++
template<typename A, typename B>
class Object
{
private:
	A num1;
	B num2;
public:
	Object(A a, B b)
		:num1(a), num2(b)
	{}

};
template<typename A>
class Object<typename A, string>
{
private:
	A num1;
	string num2;
public:
	Object(A a, string b)
		:num1(a), num2(b)
	{}

};
template<>
class Object<double, string>
{
private:
	double num1;
	string num2;
public:
	Object(double a, string b)
		:num1(a), num2(b)
	{}
};

int main()
{
	Object<int, int> obj1(1, 2);
	Object<double, int> obj2(3.14f, 2);
	Object<int, string> obj3(1, "string");// 部分特例化版本
	Object<double, string> obj4(3.14f, "string");// 完全特例化版本
	return 0;
}
```

## 继承与多态

继承是C++的重要特性，用户可以通过一个已有的类来定义一个新的类，这样做的明显好处是代码重用和执行效率的提高。当创建一个类时，用户不需要重新编写新的数据成员和成员函数，只需指定新建的类继承了一个已有的类的成员即可。这个已有的类称为**基类**，新建的类称为**派生类**。类与类之间的继承关系与现实中的继承有所区别，基类和派生类之间可以描述为“是一个”的关系，比如说基类是一个交通工具类，派生出一个汽车类，我们可以说汽车是一种交通工具，而交通工具是汽车是不准确的。

在继承中，有三点需要注意：继承权限、赋值兼容规则以及同名隐藏。一般来说公有继承最常见，保护继承和私有继承只在特定环境下使用。赋值兼容规则是公有继承的特性，其他类型的继承不适用于这条规则。下面通过一个例子来看什么是同名隐藏：

```c++
class Base
{
public:
	void fun(int a)
	{
		cout << "Base::void fun(int a)" << endl;
	}
};

class Derive : public Base
{
public:
    void fun(int a)
	{
		cout << "Derive::void fun(int a)" << endl;
	}
	void fun()
	{
		fun(10);//调用子类的fun(10)，因为父类的fun函数已经被隐藏
		Base::fun(10);//加上父类作用域后调用父类的fun(10)
	}
};

int main()
{
	Derive d;
	d.fun();
	return 0;
}
```

下面介绍多态，从字面意思上来看就是多种形态。多态可以简单地理解为同一条函数调用语句能调用不同的函数；或者说，对不同对象发送同一消息，使得不同对象有各自不同的行为。

C++中首先分为编译时多态和运行时多态。对于编译时多态，最常见的就是函数重载，采取名称粉碎技术实现，编译器可以在编译时确定调用关系。运行时多态顾名思义就是编译器在运行时才能确定调用关系，运行时多态需要三个前置条件：选择公有继承，声明虚函数，并且使用指针或者引用调用虚函数。只有满足这三个条件，编译器才会采取运行时多态的策略。首先来看下面的实例：

```c++
class Base
{
public:
	Base()
	{
		_b = 0;
		cout << "Base()" << endl;
	}
	~Base()
	{
		cout << "~Base()" << endl;
	}
	void fun(int a)
	{
		cout << "Base::void fun(int a)" << endl;
	}
private:
	int _b;
};

class Derive : public Base
{
public:
	Derive()
	{
		_d = 0;
		cout << "Derive()" << endl;
	}
	~Derive()
	{
		cout << "~Derive()" << endl;
	}
	void fun(int a)
	{
		cout << "Derive::void fun(int a)" << endl;
	}
private:
	int _d;
};

int main()
{
	Base* pb = new Derive();
	pb->fun(10);
	delete pb;
	return 0;
}
```

首先使用基类指针指向派生类对象，使用基类指针调用fun()函数。我们注意到基类和派生类中都有函数fun()，应该调用哪一个fun()呢？编译器发现调用fun()函数的指针是基类类型，调用的应该是基类中的fun()函数，所以被调用函数fun()被编译器设置为基类中的版本，这就是所谓的编译时多态，或静态联编 - 函数调用在程序执行前就准备好了。有时候这也被称为早绑定，因为fun()函数在程序编译期间就已经设置好了。对于析构函数也是同样的情况，编译器认为这是一个基类的对象，所以析构的时候只会调用基类的析构函数，实际上我们应该释放的是一个派生类的对象。那么应该如何解决上述的情况？我们在基类当中给两个函数写成虚函数即可：

```c++
virtual ~Base()
{
	cout << "~Base()" << endl;
}
virtual void fun(int a)
{
	cout << "Base::void fun(int a)" << endl;
}
```

之后再去执行刚才的操作，发现fun()函数调用的是派生类的，并且析构的时候先调用派生类的析构函数，再调用基类的析构函数，这是符合我们预期的输出。在这种情况下，编译器根据对象的类型而不是指针的类型来判断应该执行哪一种操作。如果我们现在有多个派生类继承了基类，并且都有一个fun()函数的独立实现，通过在基类中将这个函数声明为虚函数，我们可以实现根据对象类型的不同而调用不同的函数。虚函数是用来实现多态的机制，这种通过基类访问派生类成员的方式是多态的典型应用。

**虚函数**是在基类中使用关键字`virtual`声明的函数。在派生类中重新定义基类中定义的虚函数时，会告诉编译器不要静态链接到该函数。要想实现这一点，派生类中重写的虚函数需要跟基类中的虚函数同返回值、同函数名、同参数列表，否则会被识别为函数重载。我们实现的是在程序中任意点可以根据所调用的对象类型来选择调用的函数，这种操作就是运行时多态。

用户可能想要在基类中定义虚函数，以便在派生类中重新定义该函数更好地适用于对象，但是您在基类中又不能对虚函数给出有意义的实现，这个时候就会用到纯虚函数。纯虚函数一定没有定义，纯虚函数用来规范派生类的行为，即接口。包含纯虚函数的类是抽象类，抽象类不能定义实例，但可以声明指向实现该抽象类的具体类的指针或引用。

```c++
virtual void fun(int a) = 0;//纯虚函数
```

### MVC设计模式

MVC指的是model - view - controller（模型 - 视图 - 控制器），是一种混合设计模式。在下面的例子中，我们搭建一个学生管理系统的框架。作为管理系统，首先需要插入、删除、修改、查找这四个基本操作，所以我们定义一个视图类，声明一个纯虚函数process()。将视图类作为基类，派生出四个类来对应四种不同的操作。由于纯虚函数的存在，在派生类中必须有这个函数的定义，所以每一个派生类中都有process()函数的方法实现，并且都是虚函数。

然后我们定义一个控制类，最终的效果是用户可以通过选择数字的方式调用增删改查四种方法中的一种。控制类中的私有成员`_model`是一个映射表，存储我们需要的控制信息。注意映射表的类型`map<int, View*>`，键是方法的编号，通过枚举类型来实现；值是视图类的指针。观察构造函数中我们向映射表中存储的数据，可以看到键是枚举变量，而值是视图类的派生类对象。也就是说我们通过这种方式来让基类（视图类）指针指向了派生类对象。在控制类之后实现的方法中，我们首先通过用户输入的数字来查找对应的键值对，查找到后用该表项的值（基类指针）调用process()方法。由于process()是虚函数，并且是通过指针调用，符合动多态产生条件，所以编译器根据对象类型的不同而调用不同的方法，虽然方法名称都是相同的。下面是大致的框架结构，通过这种设计模式，接下来只需要在派生类中填充具体的方法实现就可以完成管理系统的主要部分。

```c++
/*MVC设计模式*/
enum TYPE
{
	INSERT,DELETE,UPDATE,SELECT
};

/*视图*/
class View
{
public:
	virtual void process() = 0;
};

class View_insert : public View
{
public:
	virtual void process()
	{
		cout << "insert" << endl;
	}
};

class View_delete : public View
{
public:
	virtual void process()
	{
		cout << "delete" << endl;
	}
};

class View_update : public View
{
public:
	virtual void process()
	{
		cout << "update" << endl;
	}
};

class View_select : public View
{
public:
	virtual void process()
	{
		cout << "select" << endl;
	}
};

/*控制*/
class Control
{
public:
	Control()
	{
		_model.insert(make_pair(INSERT, new View_insert()));
		_model.insert(make_pair(DELETE, new View_delete()));
		_model.insert(make_pair(UPDATE, new View_update()));
		_model.insert(make_pair(SELECT, new View_select()));
	}
	void process(int a)
	{
		map<int, View*>::iterator it = _model.find(a);
		if (it != _model.end())
		{
			it->second->process();
		}
	}
private:
	map<int, View*>_model;//模式
};

int main()
{
	Control _controller;
	int choice;
	while (1)
	{
		cin >> a;
		_controller.process(choice);
	}
	return 0;
}
```

## STL标准库

## 顺序容器

**vector**

向量结构类似动态开辟的数组，以两倍或者1.5倍进行扩容，只提供对尾部元素的插入和删除，不能直接进行头部的插入和删除。使用随机访问迭代器，意味着使用迭代器可以直接访问容器中任意一个元素。需要注意，由于进行扩容时 vector 会重新分配新的内存空间，所以会导致原有的迭代器或者引用类型失效。

| 成员函数         |                                                 |
| :--------------- | ----------------------------------------------- |
| vector           | 构造 `vector` (公开成员函数)                    |
| ~vector          | 析构 `vector` (公开成员函数)                    |
| operator=        | 赋值给容器 (公开成员函数)                       |
| assign           | 将值赋给容器 (公开成员函数)                     |
| **元素访问**     |                                                 |
| at               | 访问指定的元素，同时进行越界检查 (公开成员函数) |
| operator[]       | 访问指定的元素 (公开成员函数)                   |
| front            | 访问第一个元素 (公开成员函数)                   |
| back             | 访问最后一个元素 (公开成员函数)                 |
| data             | 直接访问底层数组 (公开成员函数)                 |
| **迭代器**       |                                                 |
| begin / cbegin   | 返回指向起始的迭代器 (公开成员函数)             |
| end / cend       | 返回指向末尾的迭代器 (公开成员函数)             |
| rbegin / crbegin | 返回指向起始的逆向迭代器 (公开成员函数)         |
| rend / crend     | 返回指向末尾的逆向迭代器 (公开成员函数)         |
| **容量**         |                                                 |
| empty            | 检查容器是否为空 (公开成员函数)                 |
| size             | 返回容纳的元素数 (公开成员函数)                 |
| max_size         | 返回可容纳的最大元素数 (公开成员函数)           |
| reserve          | 预留存储空间 (公开成员函数)                     |
| capacity         | 返回当前存储空间能够容纳的元素数 (公开成员函数) |
| **修改器**       |                                                 |
| clear            | 清除内容 (公开成员函数)                         |
| insert           | 插入元素 (公开成员函数)                         |
| emplace          | 原位构造元素 (公开成员函数)                     |
| erase            | 擦除元素 (公开成员函数)                         |
| push_back        | 将元素添加到容器末尾 (公开成员函数)             |
| emplace_back     | 在容器末尾就地构造元素 (公开成员函数)           |
| pop_back         | 移除末元素 (公开成员函数)                       |
| resize           | 改变容器中可存储元素的个数 (公开成员函数)       |
| swap             | 交换内容 (公开成员函数)                         |

**list**

采用一种双向循环链表结构，初始状态仅含有头结点，不需要考虑扩容等问题。使用双向迭代器，也就是说迭代器只能按顺序依次遍历每一个元素。一般使用迭代器遍历 list 容器，其中 begin() 方法使得迭代器指向第一个有效结点，而 end() 方法指向头结点。

| 成员函数           |                                             |
| ------------------ | ------------------------------------------- |
| list               | 构造 `list` (公开成员函数)                  |
| ~list              | 析构 `list` (公开成员函数)                  |
| operator=          | 赋值给容器 (公开成员函数)                   |
| assign             | 将值赋给容器 (公开成员函数)                 |
| **元素访问**       |                                             |
| front              | 访问第一个元素 (公开成员函数)               |
| back               | 访问最后一个元素 (公开成员函数)             |
| **迭代器**         |                                             |
| begin / cbegin     | 返回指向起始的迭代器 (公开成员函数)         |
| end / cend         | 返回指向末尾的迭代器 (公开成员函数)         |
| rbegin / crbegin   | 返回指向起始的逆向迭代器 (公开成员函数)     |
| rend / crend       | 返回指向末尾的逆向迭代器 (公开成员函数)     |
| **容量**           |                                             |
| empty              | 检查容器是否为空 (公开成员函数)             |
| size               | 返回容纳的元素数 (公开成员函数)             |
| max_size           | 返回可容纳的最大元素数 (公开成员函数)       |
| **修改器**         |                                             |
| clear              | 清除内容 (公开成员函数)                     |
| insert             | 插入元素 (公开成员函数)                     |
| emplace            | 原位构造元素 (公开成员函数)                 |
| erase              | 擦除元素 (公开成员函数)                     |
| push_back          | 将元素添加到容器末尾 (公开成员函数)         |
| emplace_back       | 在容器末尾就地构造元素 (公开成员函数)       |
| pop_back           | 移除末元素 (公开成员函数)                   |
| push_front         | 插入元素到容器起始 (公开成员函数)           |
| emplace_front      | 在容器头部原位构造元素 (公开成员函数)       |
| pop_front          | 移除首元素 (公开成员函数)                   |
| resize             | 改变容器中可存储元素的个数 (公开成员函数)   |
| swap               | 交换内容 (公开成员函数)                     |
| **操作**           |                                             |
| merge              | 合并二个已排序列表 (公开成员函数)           |
| splice             | 从另一个`list`中移动元素 (公开成员函数)     |
| remove / remove_if | 移除满足特定标准的元素 (公开成员函数)       |
| reverse            | 将该链表的所有元素的顺序反转 (公开成员函数) |
| unique             | 删除连续的重复元素 (公开成员函数)           |
| sort               | 对元素进行排序 (公开成员函数)               |

**deque**

deque（ double-ended queue ，双端队列）是有下标顺序容器，它允许在其首尾两端快速插入及删除。使用随机访问迭代器。与 vector 相反， deque 的元素不是连续存储的：典型实现用单独分配的固定大小数组的序列，外加额外的指针登记，这表示下标访问必须进行二次指针解引用，与之相比 vector 的下标访问只进行一次。deque 的存储按需自动扩展及收缩。扩张 deque 比扩张 vector 更优，因为它不涉及到复制既存元素到新内存位置。

| 成员函数         |                                                 |
| ---------------- | ----------------------------------------------- |
| deque            | 构造 `deque` (公开成员函数)                     |
| ~deque           | 析构 `deque` (公开成员函数)                     |
| operator=        | 赋值给容器 (公开成员函数)                       |
| assign           | 将值赋给容器 (公开成员函数)                     |
| **元素访问**     |                                                 |
| at               | 访问指定的元素，同时进行越界检查 (公开成员函数) |
| operator[]       | 访问指定的元素 (公开成员函数)                   |
| front            | 访问第一个元素 (公开成员函数)                   |
| back             | 访问最后一个元素 (公开成员函数)                 |
| **迭代器**       |                                                 |
| begin / cbegin   | 返回指向起始的迭代器 (公开成员函数)             |
| end / cend       | 返回指向末尾的迭代器 (公开成员函数)             |
| rbegin / crbegin | 返回指向起始的逆向迭代器 (公开成员函数)         |
| rend / crend     | 返回指向末尾的逆向迭代器 (公开成员函数)         |
| **容量**         |                                                 |
| empty            | 检查容器是否为空 (公开成员函数)                 |
| size             | 返回容纳的元素数 (公开成员函数)                 |
| max_size         | 返回可容纳的最大元素数 (公开成员函数)           |
| **修改器**       |                                                 |
| clear            | 清除内容 (公开成员函数)                         |
| insert           | 插入元素 (公开成员函数)                         |
| emplace          | 原位构造元素 (公开成员函数)                     |
| erase            | 擦除元素 (公开成员函数)                         |
| push_back        | 将元素添加到容器末尾 (公开成员函数)             |
| emplace_back     | 在容器末尾就地构造元素 (公开成员函数)           |
| pop_back         | 移除末元素 (公开成员函数)                       |
| push_front       | 插入元素到容器起始 (公开成员函数)               |
| emplace_front    | 在容器头部原位构造元素 (公开成员函数)           |
| pop_front        | 移除首元素 (公开成员函数)                       |
| resize           | 改变容器中可存储元素的个数 (公开成员函数)       |
| swap             | 交换内容 (公开成员函数)                         |

## 关联容器

**set**

set 是关联容器，包含有 key 类型对象的已排序集，使用双向迭代器。搜索、移除和插入拥有对数复杂度。 set 通常以红黑树实现。

| 成员函数         |                                                         |
| ---------------- | ------------------------------------------------------- |
| set              | 构造 `set` (公开成员函数)                               |
| ~set             | 析构 `set` (公开成员函数)                               |
| operator=        | 赋值给容器 (公开成员函数)                               |
| **迭代器**       |                                                         |
| begin / cbegin   | 返回指向起始的迭代器 (公开成员函数)                     |
| end / cend       | 返回指向末尾的迭代器 (公开成员函数)                     |
| rbegin / crbegin | 返回指向起始的逆向迭代器 (公开成员函数)                 |
| rend / crend     | 返回指向末尾的逆向迭代器 (公开成员函数)                 |
| **容量**         |                                                         |
| empty            | 检查容器是否为空 (公开成员函数)                         |
| size             | 返回容纳的元素数 (公开成员函数)                         |
| max_size         | 返回可容纳的最大元素数 (公开成员函数)                   |
| **修改器**       |                                                         |
| clear            | 清除内容 (公开成员函数)                                 |
| insert           | 插入元素或结点 (C++17 起) (公开成员函数)                |
| emplace          | 原位构造元素 (公开成员函数)                             |
| erase            | 擦除元素 (公开成员函数)                                 |
| swap             | 交换内容 (公开成员函数)                                 |
| merge            | 从另一容器接合结点 (公开成员函数)                       |
| **查找**         |                                                         |
| count            | 返回匹配特定键的元素数量 (公开成员函数)                 |
| find             | 寻找带有特定键的元素 (公开成员函数)                     |
| equal_range      | 返回匹配特定键的元素范围 (公开成员函数)                 |
| lower_bound      | 返回指向首个*不小于*给定键的元素的迭代器 (公开成员函数) |
| upper_bound      | 返回指向首个*大于*给定键的元素的迭代器 (公开成员函数)   |

**multiset**

multiset 是含有 key 类型对象有序集的容器。不同于 set ，它允许多个 key 拥有等价的值。

**map**

map 是有序键值对容器，它的元素的键是唯一的，使用双向迭代器。搜索、移除和插入操作拥有对数复杂度。 map 通常实现为红黑树。

| 成员函数         |                                                         |
| ---------------- | ------------------------------------------------------- |
| map              | 构造 `map` (公开成员函数)                               |
| ~map             | 析构 `map` (公开成员函数)                               |
| operator=        | 赋值给容器 (公开成员函数)                               |
| **元素访问**     |                                                         |
| at               | 访问指定的元素，同时进行越界检查 (公开成员函数)         |
| operator[]       | 访问或插入指定的元素 (公开成员函数)                     |
| **迭代器**       |                                                         |
| begin / cbegin   | 返回指向起始的迭代器 (公开成员函数)                     |
| end / cend       | 返回指向末尾的迭代器 (公开成员函数)                     |
| rbegin / crbegin | 返回指向起始的逆向迭代器 (公开成员函数)                 |
| rend / crend     | 返回指向末尾的逆向迭代器 (公开成员函数)                 |
| **容量**         |                                                         |
| empty            | 检查容器是否为空 (公开成员函数)                         |
| size             | 返回容纳的元素数 (公开成员函数)                         |
| max_size         | 返回可容纳的最大元素数 (公开成员函数)                   |
| **修改器**       |                                                         |
| clear            | 清除内容 (公开成员函数)                                 |
| insert           | 插入元素或结点 (C++17 起) (公开成员函数)                |
| emplace          | 原位构造元素 (公开成员函数)                             |
| erase            | 擦除元素 (公开成员函数)                                 |
| swap             | 交换内容 (公开成员函数)                                 |
| merge            | 从另一容器接合结点 (公开成员函数)                       |
| **查找**         |                                                         |
| count            | 返回匹配特定键的元素数量 (公开成员函数)                 |
| find             | 寻找带有特定键的元素 (公开成员函数)                     |
| equal_range      | 返回匹配特定键的元素范围 (公开成员函数)                 |
| lower_bound      | 返回指向首个*不小于*给定键的元素的迭代器 (公开成员函数) |
| upper_bound      | 返回指向首个*大于*给定键的元素的迭代器 (公开成员函数)   |

**multimap**

multimap 是关联容器，含有键值对的已排序列表，同时允许多个元素拥有同一个键。

## 容器适配器

容器适配器考虑将STL基本容器进一步封装，进而产生一类新的容器。这类容器是基本容器的特殊化、专门化形式，专门用于满足特定场景需求。容器适配器的原理可以推广到迭代器适配器，一类采用同样思想的迭代器封装形式。

**queue**

一种容器适配器，采取先进先出的原则，默认使用的底层容器是 deque，也可以是 list，但不能是 vector，因为 vector 不支持头部操作。

| 成员函数     |                                     |
| ------------ | ----------------------------------- |
| queue        | 构造 `queue` (公开成员函数)         |
| ~queue       | 析构 `queue` (公开成员函数)         |
| operator=    | 赋值给容器适配器 (公开成员函数)     |
| **元素访问** |                                     |
| front        | 访问第一个元素 (公开成员函数)       |
| back         | 访问最后一个元素 (公开成员函数)     |
| **容量**     |                                     |
| empty        | 检查底层容器是否为空 (公开成员函数) |
| size         | 返回容纳的元素数 (公开成员函数)     |
| **修改器**   |                                     |
| push         | 向队列尾部插入元素 (公开成员函数)   |
| emplace      | 在尾部原位构造元素 (公开成员函数)   |
| pop          | 删除首个元素 (公开成员函数)         |
| swap         | 交换内容 (公开成员函数)             |

**stack**

一种容器适配器，采用后进先出原则，默认的底层容器是 deque，也可以使用 vector 或 list 作为其底层容器。

| 成员函数     |                                     |
| ------------ | ----------------------------------- |
| stack        | 构造 `stack` (公开成员函数)         |
| ~stack       | 析构 `stack` (公开成员函数)         |
| operator=    | 赋值给容器适配器 (公开成员函数)     |
| **元素访问** |                                     |
| top          | 访问栈顶元素 (公开成员函数)         |
| **容量**     |                                     |
| empty        | 检查底层容器是否为空 (公开成员函数) |
| size         | 返回容纳的元素数 (公开成员函数)     |
| **修改器**   |                                     |
| push         | 向栈顶插入元素 (公开成员函数)       |
| emplace      | 在顶部原位构造元素 (公开成员函数)   |
| pop          | 删除栈顶元素 (公开成员函数)         |
| swap         | 交换内容 (公开成员函数)             |

**priority_queue**

同样是一种容器适配器，默认底层容器为 vector。采用数组的方式实现了一个堆有序的完全二叉树结构，可以做到删除最大元素和插入元素。要求迭代器必须是随机访问属性，所以底层容器不能是 list，但可以选择 deque。

| 成员函数        |                                      |
| --------------- | ------------------------------------ |
| priority_queue  | 构造 `priority_queue` (公开成员函数) |
| ~priority_queue | 析构 `priority_queue` (公开成员函数) |
| operator=       | 赋值给容器适配器 (公开成员函数)      |
| **元素访问**    |                                      |
| top             | 访问栈顶元素 (公开成员函数)          |
| **容量**        |                                      |
| empty           | 检查底层容器是否为空 (公开成员函数)  |
| size            | 返回容纳的元素数 (公开成员函数)      |
| **修改器**      |                                      |
| push            | 向栈顶插入元素 (公开成员函数)        |
| emplace         | 在顶部原位构造元素 (公开成员函数)    |
| pop             | 删除栈顶元素 (公开成员函数)          |
| swap            | 交换内容 (公开成员函数)              |

## 迭代器适配器

基本迭代器大致分为输入迭代器、输出迭代器、前向迭代器、双向迭代器、随机迭代器，后三类迭代器广泛应用于STL的标准容器中，同容器适配器一样，标准迭代器需要一类封装形式来实现特化，用以处理解决特定场景下的需求，这就是迭代器适配器。其中主要介绍逆向迭代器、插入迭代器以及流迭代器。

### 逆向迭代器（reverse_iterator）

逆向迭代器逆向访问容器，根据容器内元素属性分为常性和非常性。

```c++
// 反向输出
template<class T>
void print_reverse(T& t)
{
	typename T::reverse_iterator it = t.rbegin();
	for (;it != t.rend();it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

// 常量反向输出
template<class T>
void print_const_reverse(const T& t)
{
	typename T::const_reverse_iterator it = t.crbegin();
	for (;it != t.crend();it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}
```

### 插入型迭代器（insert_iterator）

**insert_iterator**

普通插入型迭代器通过调用容器的`insert()`方法实现插入，用户指定插入位置。所有标准容器均适合于此迭代器，因为所有容器均定义了`insert()`方法。所有插入型迭代器的基本功能是**将一个赋值操作转换为把相应的值插入进容器中**，由此我们可以实现一个类似拷贝操作的模板函数。指定数据的开头和结尾，通过插入型迭代器将数据插入，达到拷贝的效果。

```c++
/*Insert函数*/
template<typename INSERT_IT,typename IT>
void Insert(const IT& first, const IT& end, INSERT_IT insert)
{
	IT tmp = first;
	for (;tmp != end;tmp++)
	{
		*insert = *tmp;// 将first到end的内容插入到insert_iterator
	}
}
```

通过上面的函数，我们可以完成用一个容器去初始化另一个容器的操作，就像下面这样：

```c++
/*Insert函数使用例*/
vector<int> v1 = {1,2,3,4,5};
vector<int> v2;
insert_iterator<vector<int>> insertit(v2,v2.begin());
Insert(v1.begin(), v1.end(), insertit);
```

以上介绍的流程是首先实例化一个`insert_iterator`类的对象，然后将这个对象作为参数传入`Insert()`的方法。还可以使用`inserter()`函数，这个函数返回值是一个`insert_iterator`类的对象，所以上面调用`Insert()`也可以写成下面的形式：

```c++
Insert(v1.begin(), v1.end(), inserter(v2,v2.begin()));
```

这种形式和之前的区别是第三个参数直接调用`inserter()`函数，该函数是一种通用的方法，在很多迭代器中都有实现。`inserter()`方法的思想很简单，就是在函数中帮助我们实例化了一个`insert_iterator`类的对象并返回。如下面的函数所示：

```c++
/*inserter函数*/
template<typename CON>
insert_iterator<CON>minserter(CON& con, typename CON::iterator it)
{
	return insert_iterator<CON>(con, it);
}
```

**back_insert_iterator**

`back_insert_iterator`是`insert_iterator`的变种，就是只从尾部插入的迭代器。底层调用的是所属对象的`push_back()`方法，因此这种插入器只对 vector ,list, deque 和 string 有效。

```c++
/*back_inserter函数*/
template<typename CON>
insert_iterator<CON>mback_inserter(CON& con)
{
	return back_insert_iterator<CON>(con);
}
```

**front_insert_iterator**

`front_insert_iterator`是从头部插入的迭代器，底层调用的是所属对象的`push_front()`方法，因此只对 list 和 deque 有效。

```c++
/*front_inserter函数*/
template<typename CON>
front_iterator<CON>mfront_inserter(CON& con)
{
	return front_insert_iterator<CON>(con);
}
```

### 流迭代器

流迭代器分为输出流迭代器和输入流迭代器，`ostream_iterator`和`istream_iterator`。使用时首先指定模板类型参数，然后流迭代器一定要绑定流，对于输出流迭代器还可以进行格式控制，如下所示：

```c++
ostream_iterator<int> ito(cout, " ");// 指定输出类型为整型，绑定标准输出流，设定输出元素用空格间隔
istream_iterator<double> iti(cin);// 指定输入类型为浮点型，绑定标准输入流
```

**ostream_iterator使用例**

```c++
list<int> mylist;// 定义一个list容器类对象
ostream_iterator<int> out(cout, " ");// 定义一个输出流迭代器对象
for (int i = 0;i < 10;i++)
{
	mylist.push_back(i);
}
Insert(mylist.begin(), mylist.end(), out);// 将mylist的内容输出到屏幕上
```

可以看到我们输出对象中的元素时使用了之前提到的`Insert()`方法，实际上这个函数是将对象中的所有元素插入到了输出流迭代器当中，所以输出流迭代器可以也可以看做是一种插入型迭代器。

**istream_iterator使用例**

```c++
cout << "Enter one or more words. Enter ! to end:" << endl;
istream_iterator<string> in(cin);
vector<string> words;
while (1)
{
	string word = *in;
	if (word == "!")break;
	words.push_back(word);
	in++;
}
cout << "You entered " << words.size() << " words." << endl;
```

上例使用流迭代器读取字符串。循环从标准输入流中读取单词，并把它们添加到 vector 容器中，直到按下回车键。表达式 *in 的值是从底层流读到的当前 string 对象。++in 会导致从流中读取下一个 string 对象，并保存到这个迭代器中。

```c++
std::cout << "Enter some integers - enter Ctrl+Z to end.\n";
std::istream_iterator<int> it{ std::cin };         // Create begin input stream iterator
std::istream_iterator<int> end_iter;                 // Create end input stream iterator
// Read some integers to sum
int sum = 0;
while (iter != end_iter)                             // Continue until Ctrl+Z read
{
	sum += *iter++;
}
std::cout << "Total is " << sum << std::endl;
```

在显示输入提示后，我们可以用一个输入流迭代器从输入流中读取整型值，和一个结束迭代器对象。循环会求出所有用输入流迭代器读入的值的和，直到识别出 EOF 状态，它是通过从流中读取 Ctrl+Z 标志设置的。解引用 it 可以使它所指向的值变得可用，之后递增运算会移动 it，使它指向下一个输入。如果输入的是 Ctrl+Z，循环结束。

## 泛型算法

通过一个接口操纵各种数据类型的算法叫做泛型算法，这种算法基于迭代器而使用，主要针对容器进行操作。这是因为容器本身就可以存储各种数据类型，并且都拥有自己的迭代器。使用算法库需要`#include<algorithm>`。在这之前我们已经看到了使用迭代器的好处，即做到了忽略不同容器内部不同的构造，可以通过统一的方法操纵各种容器的数据。但是我们同样也发现，仅仅通过目前的手段只能做到对容器基本的增删改查，比如说为容器排序或者查找容器中某个元素的时候还是需要用户自己编写算法实现，并不是十分方便，泛型算法就是为了解决这个问题而出现。泛型算法将所有类型的容器中算法部分抽象出来，整合成一个库，用户通过调用这些统一的接口，就可以实现对各种类型容器更进一步的操作。下面介绍几种泛型算法，为了体现算法本身的功能，我们使用存储整型的向量容器作为例子。

### copy

`copy()`函数接收三个参数，前两个参数指定范围，第三个参数指定位置，功能就是将指定范围中的所有数据拷贝到某个指定位置。这个算法应用广泛，我们可以进行如下的操作：

```c++
vector<int> v1 = { 4,3,8,5,2,1,6,7,9,10 };
copy(v1.begin(), v1.end(), ostream_iterator<int>(cout, " "));// 输出v1所有元素
vector<int> v2;
copy(v1.begin(), v1.end(), inserter(v2, v2.begin()));// 用v1初始化v2
```

下面是`copy()`方法一种可能的实现方式：

```c++
template<class InputIt, class OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
{
    while (first != last) {
        *d_first++ = *first++;
    }
    return d_first;
}
```

### find

`find()`函数接收三个参数，前两个指定范围，第三个参数指定查找元素，功能是在指定范围查找第一个符合要求的元素，该函数会返回一个迭代器，查找成功时，其指向的是在 [first, last) 区域内查找到的第一个目标元素；如果查找失败，则该迭代器的指向和 last 相同。下面是一个简单的查找应用：

```c++
vector<int>::iterator it = find(v1.begin(), v1.end(), 5);
if (it == v1.end())
{
	cout << "not find" << endl;
}
else
{
	cout << "find number: " << *it << endl;
}
```

下面是`find()`函数的一种可能的实现方式：

```c++
template<class InputIt, class T>
InputIt find(InputIt first, InputIt last, const T& value)
{
    for (; first != last; ++first) {
        if (*first == value) {
            return first;
        }
    }
    return last;
}
```

### sort

`sort()` 函数本质是一个模板函数。该函数专门用来对容器或普通数组中指定范围内的元素进行排序，排序规则默认以元素值的大小做升序排序，除此之外我们也可以选择标准库提供的其它排序规则（比如`std::greater<T>`降序排序规则），甚至还可以自定义排序规则。

需要注意的是，`sort()`函数受到底层实现方式的限制，它仅适用于普通数组和部分类型的容器。换句话说，只有普通数组和具备以下条件的容器，才能使用 `sort()`函数：

- 容器支持的迭代器类型必须为随机访问迭代器。这意味着，`sort()`只对 array、vector、deque 这 3 个容器提供支持。
- 如果对容器中指定区域的元素做默认升序排序，则元素类型必须支持`<`小于运算符；同样，如果选用标准库提供的其它排序规则，元素类型也必须支持该规则底层实现所用的比较运算符。
- `sort()`函数在实现排序时，需要交换容器中元素的存储位置。这种情况下，如果容器中存储的是自定义的类对象，则该类的内部必须提供移动构造函数和移动赋值运算符。

对于自定义规则，用户可以使用不带参数的默认规则；显式给出标准库提供的规则；定义函数对象，自定义规则；使用 lambda 表达式。

```c++
    std::vector<int> s = { 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };

	// 用默认的 operator< 排序
	std::sort(s.begin(), s.end());
	for (auto a : s) {
		std::cout << a << " ";
	}
	std::cout << '\n';

	// 用标准库比较函数对象排序
	std::sort(s.begin(), s.end(), std::greater<int>());
	for (auto a : s) {
		std::cout << a << " ";
	}
	std::cout << '\n';

	// 用自定义函数对象排序
	struct {
		bool operator()(int a, int b) const
		{
			return a < b;
		}
	} customLess;
	std::sort(s.begin(), s.end(), customLess);
	for (auto a : s) {
		std::cout << a << " ";
	}
	std::cout << '\n';

	// 用 lambda 表达式排序
	std::sort(s.begin(), s.end(), [](int a, int b) {
		return b < a;
		});
	for (auto a : s) {
		std::cout << a << " ";
	}
	std::cout << '\n';
```

下面是`sort()`函数的一种可能的实现方式。简单起见仅使用选择排序作为排序算法，参数 pre 是用户给出的规则：

```c++
template<class IT,class PR>
void msort(const IT& first, const IT& last, PR pre)
{
	// order [first, last), using pre
	for (IT tmp1 = first; tmp1 != last; tmp1++)
	{
		for (IT tmp2 = tmp1 + 1; tmp2 != last; tmp2++)
		{
			if (!pre(*tmp1, *tmp2))
			{
				std::swap(*tmp1, *tmp2);
			}
		}
	}
}
```

copy 算法是一种修改序列的操作，find 算法显然是不修改序列的操作，sort 是一种排序操作。STL库中提供大量的算法模板，这里列出的算法仅仅是最为简单的三个算法，更多详细的算法请参见 [算法库 - cppreference.com](https://zh.cppreference.com/w/cpp/algorithm)。

## 智能指针

智能指针本身不是指针，本质上是封装了指针的类，使用时加上`#include<memory>`。用户对于系统内存空间的管理往往分为三个阶段：申请，使用，归还。一般来说，用户通过指针进行申请的堆上空间，或者文件资源都需要手动释放，在很多场景下，资源何时应该被释放难以判断，或者程序员忘记释放自己申请的资源，产生内存泄露。使用智能指针将简化这一过程，智能指针遵循RAII思想，简单来说就是堆上空间进行自动化管理，利用对象自动析构的机制来实现。

### auto_ptr（废弃）

auto_ptr 是最早的智能指针，常用的使用方式如下：

```c++
int* p = new int(10);
auto_ptr<int> a_p1(p);//OK，使用普通指针初始化
auto_ptr<int> a_p2(new int(10));//OK，直接初始化
auto_ptr<int> a_p3 = p;//ERR，不允许产生隐式构造
auto_ptr<int> a_p4 = new int(10);//ERR，不允许产生隐式构造
auto_ptr<int> a_p5 = (auto_ptr<int>)p;//OK，使用显式构造
auto_ptr<int> a_p6 = a_p1;//OK，拷贝构造，但是a_p1置空
a_p6 = a_p1;//OK，等号运算符重载，但是a_p1置空
```

auto_ptr 有三个特点：不能使用同一个裸指针赋值或者初始化多个 auto_ptr，否则会引发堆空间被重复释放；不允许裸指针对auto_ptr 进行隐式构造，这是由 auto_ptr 构造函数`explicit`关键字规定；允许使用拷贝构造，允许使用等号运算符重载，但会将原智能指针置空。

下面是 auto_ptr 源码的简单实现：

```c++
template<class _Ty>
class my_auto_ptr
{
private:
	bool _Owns;
	_Ty* _Ptr;
public:
	my_auto_ptr(_Ty* p = nullptr) :_Owns(p != nullptr), _Ptr(p) {}
	~my_auto_ptr()
	{
		if (_Owns)delete _Ptr;
		_Owns = false;
		_Ptr = nullptr;
	}
	my_auto_ptr(const my_auto_ptr& _Y) :_Owns(_Y._Owns), _Ptr(_Y.release()) {}
	my_auto_ptr& operator=(const my_auto_ptr& _Y)
	{
		if (this == &_Y)return *this;
		if (_Owns)
		{
			delete _Ptr;
		}
		_Owns = _Y._Owns;
		_Ptr = _Y.release();
		return *this;
	}
	_Ty* get() const { return _Ptr; }
	_Ty& operator*() const
	{
		return *get();
	}
	_Ty* operator->() const
	{
		return get();
	}
	void reset(_Ty* p = nullptr)
	{
		if (_Owns)
		{
			delete _Ptr;
		}
		_Ptr = p;
	}
	_Ty* release() const
	{
		_Ty* tmp = nullptr;
		if (_Owns)
		{
			((my_auto_ptr*)this)->_Owns = false;
			tmp = _Ptr;
			((my_auto_ptr*)this)->_Ptr = nullptr;
		}
		return tmp;
	}
};
```

首先由于 auto_ptr 的特性，将同一个对象的地址初始化或者赋值多个 auto_ptr 将会导致程序的崩溃，虽然拷贝构造函数和赋值运算符重载函数通过转让资源的方式避免了这种情况，但是在特定场景下会出现意外。试想如果我们将主函数中的 auto_ptr 通过值传递方式赋值给函数形参，那么函数体内的临时变量会正常使用并释放资源，但是主函数中的 auto_ptr 已经丢失了资源，这并不是我们我们所要达到的目的。并且应该注意到，我们的智能指针目前只能应对单一对象，如果开辟的是一组对象，上面的程序将不能正常运行。由于 auto_ptr 有众多限制，并且存在缺陷，所以已经废弃，现在有三种改进版本。

### unique_ptr

unique_ptr 同样不能使用同一个裸指针赋值或者初始化多个 unique_ptr；不允许裸指针对 unique_ptr 进行隐式构造；和 auto_ptr 不同的是，unique_ptr 不允许拷贝构造，不允许赋值运算符重载，但允许移动拷贝构造，移动赋值运算符重载。

下面是 unique_ptr 的源码实现：

```c++
template<typename _Ty>
class MyDeletor
{
public:
	MyDeletor() = default;
	void operator()(_Ty* ptr)const
	{
		if (ptr != nullptr)
		{
			delete ptr;
		}
	}
};
template<class _Ty>
class MyDeletor<_Ty[]>
{
public:
	MyDeletor() = default;
	void operator()(_Ty* ptr)const
	{
		if (ptr != nullptr)
		{
			delete[] ptr;
		}
	}
};

template<class _Ty, class _Dx = MyDeletor<_Ty>>
class my_unique_ptr
{
public:
	using pointer = _Ty*;
	using element_type = _Ty;
	using delete_type = _Dx;
private:
	_Ty* _Ptr;
	_Dx _myDeletor;
public:
	my_unique_ptr(const my_unique_ptr&) = delete;
	my_unique_ptr& operator=(const my_unique_ptr&) = delete;
	my_unique_ptr(pointer _P = nullptr) :_Ptr(_P) {}
	~my_unique_ptr()
	{
		if (_Ptr != nullptr)
		{
			_myDeletor(_Ptr);
			_Ptr = nullptr;
		}
	}
	my_unique_ptr(my_unique_ptr&& _Y)// 移动拷贝
	{
		_Ptr = _Y._Ptr;
		_Y._Ptr = nullptr;
	}
	my_unique_ptr& operator=(my_unique_ptr&& _Y)// 移动赋值
	{
		if (this == &_Y)return *this;
		reset(_Y.release());
		return *this;
	}
	_Dx& get_deleter()// 获取删除器
	{
		return _myDeletor;
	}
	const _Dx& get_deleter()const
	{
		return _myDeletor;
	}
	_Ty& operator*()const
	{
		return *_Ptr;
	}
	pointer operator->()const
	{
		return &**this;
	}
	pointer get()const// 获取对象地址
	{
		return _Ptr;
	}
	operator bool()const
	{
		return _Ptr != nullptr;
	}
	pointer release()
	{
		_Ty* old = _Ptr;
		_Ptr = nullptr;
		return old;
	}
	void reset(pointer _P = nullptr)
	{
		pointer _old = _Ptr;
		_Ptr = _P;

		if (_old != nullptr)
		{
			_myDeletor(_old);
		}
	}
	void swap(my_unique_ptr _Y)
	{
		std::swap(_Ptr, _Y._Ptr);
		std::swap(_myDeletor, _Y._myDeletor);
	}
};

template<class _Ty, class _Dx>
class my_unique_ptr<_Ty[], _Dx>
{
public:
	using pointer = _Ty*;
	using element_type = _Ty;
	using delete_type = _Dx;
private:
	_Ty* _Ptr;
	_Dx _myDeletor;
public:
	my_unique_ptr(const my_unique_ptr&) = delete;
	my_unique_ptr& operator=(const my_unique_ptr&) = delete;
	my_unique_ptr(pointer _P = nullptr) :_Ptr(_P) {}
	~my_unique_ptr()
	{
		if (_Ptr != nullptr)
		{
			_myDeletor(_Ptr);
			_Ptr = nullptr;
		}
	}
	my_unique_ptr(my_unique_ptr&& _Y)// 移动拷贝
	{
		_Ptr = _Y._Ptr;
		_Y._Ptr = nullptr;
	}
	my_unique_ptr& operator=(my_unique_ptr&& _Y)// 移动赋值
	{
		if (this == &_Y)return *this;
		reset(_Y.release());
		return *this;
	}
	_Dx& get_deleter()// 获取删除器
	{
		return _myDeletor;
	}
	const _Dx& get_deleter()const
	{
		return _myDeletor;
	}
	_Ty& operator*()const
	{
		return *_Ptr;
	}
	pointer operator->()const
	{
		return &**this;
	}
	pointer get()const// 获取对象地址
	{
		return _Ptr;
	}
	operator bool()const
	{
		return _Ptr != nullptr;
	}
	pointer release()
	{
		_Ty* old = _Ptr;
		_Ptr = nullptr;
		return old;
	}
	void reset(pointer _P = nullptr)
	{
		pointer _old = _Ptr;
		_Ptr = _P;

		if (_old != nullptr)
		{
			_myDeletor(_old);
		}
	}
	void swap(my_unique_ptr _Y)
	{
		std::swap(_Ptr, _Y._Ptr);
		std::swap(_myDeletor, _Y._myDeletor);
	}
	_Ty& operator[](const int _Idx)const
	{
		return _Ptr[_Idx];
	}
};

template<class _Ty, class... _Types>
my_unique_ptr<_Ty>my_make_unique(_Types&&... _Args)
{
	return my_unique_ptr<_Ty>(new _Ty(_Args...));
}
```

上述程序应该指出的是，unique_ptr 改进了前代智能指针的功能，首先删除了普通的拷贝构造函数以及赋值运算符重载，杜绝了两个函数所带来的隐患，允许使用移动拷贝构造函数和移动赋值运算符重载，因为转移临时量的资源并不会对程序造成影响。定义了删除器，专门用于删除资源的类，并细分为删除单一对象的删除器和删除一组对象的删除器，具体应该使用哪个删除器取决于我们使用哪一个类模板。我们有一个部分特例化的类模板专门用于管理指向一组对象的智能指针，当程序中申请了一组对象时，优先调用该模板。

### shared_ptr

shared_ptr 也叫做强智能指针，不能使用同一个裸指针赋值或者初始化多个 shared_ptr，不允许隐式构造，但是允许使用拷贝构造函数和等号运算符重载，并且不会将原先的智能指针置空。

下面是 shared_ptr 的源码实现：

```c++
template<class _Ty>
class MyDeletor
{
public:
	MyDeletor() = default;
	void operator()(_Ty* ptr) const
	{
		if (ptr != nullptr)
		{
			delete ptr;
		}
	}
};
template<class _Ty>
class MyDeletor<_Ty[]>
{
public:
	MyDeletor() = default;
	void operator()(_Ty* ptr) const
	{
		if (ptr != nullptr)
		{
			delete[]ptr;
		}
	}
};
template<class _Ty>
class RefCnt// 引用计数结构
{
public:
	_Ty* mptr;
	int ref;

public:
	RefCnt(_Ty* p = nullptr) :mptr(p), ref(mptr != nullptr) {}
	~RefCnt() {}
};
template<class _Ty, class _Dx = MyDeletor<_Ty> >
class my_shared_ptr
{
public:
	my_shared_ptr(_Ty* p = nullptr) :ptr(nullptr)
	{
		if (p != nullptr)
		{
			ptr = new RefCnt<_Ty>(p);
		}
	}
	my_shared_ptr(const my_shared_ptr& _Y) :ptr(_Y.ptr)
	{
		if (ptr != nullptr)
		{
			ptr->ref += 1;
		}
	}
	my_shared_ptr(my_shared_ptr&& _Y) :ptr(_Y.ptr)
	{
		_Y.ptr = nullptr;
	}
	operator bool() const { return ptr != nullptr; }

	my_shared_ptr& operator=(const my_shared_ptr& _Y)
	{
		if (this == &_Y || this->ptr == _Y.ptr) return *this;
		if (ptr != NULL && --ptr->ref == 0)
		{
			mDeletor(ptr->mptr);
			delete ptr;
		}
		ptr = _Y.ptr;
		if (ptr != nullptr)
		{
			ptr->ref += 1;
		}
		return *this;
	}
	my_shared_ptr& operator=(my_shared_ptr&& _Y)
	{
		if (this == &_Y) return *this;
		if (this->ptr == _Y.ptr && this->ptr != nullptr && _Y.ptr != nullptr)
		{
			this->ptr->ref -= 1;
			_Y.ptr = nullptr;
			return *this;
		}
		if (this->ptr != nullptr && --ptr->ref == 0)
		{
			mDeletor(ptr->mptr);
			delete ptr;
		}
		ptr = _Y.ptr;
		_Y.ptr = nullptr;
		return *this;
	}
	void reset(_Ty* p = nullptr)
	{
		if (this->ptr != nullptr && --this->ptr->ref == 0)
		{
			mDeletor(ptr->mptr);
			delete ptr;
		}
		ptr = new RefCnt<_Ty>(p);
	}
	~my_shared_ptr()
	{
		if (this->ptr != nullptr && --this->ptr->ref == 0)
		{
			mDeletor(ptr->mptr);
			delete ptr;
		}
		ptr = nullptr;
	}
	_Ty* get() const { return ptr->mptr; }
	_Ty& operator*() const
	{
		return *get();
	}
	_Ty* operator->() const
	{
		return get();
	}

	size_t use_count() const
	{
		if (this->ptr == nullptr) return 0;
		return this->ptr->ref;
	}
	void swap(my_shared_ptr& r)
	{
		std::swap(this->ptr, r.ptr);
	}

private:
	RefCnt<_Ty>* ptr;
	_Dx mDeletor;
};

template<class _Ty, class _Dx >
class my_shared_ptr<_Ty[], _Dx>
{
public:
	my_shared_ptr(_Ty* p = nullptr) :ptr(nullptr)
	{
		if (p != nullptr)
		{
			ptr = new RefCnt<_Ty>(p);
		}
	}
	my_shared_ptr(const my_shared_ptr& _Y) :ptr(_Y.ptr)
	{
		if (ptr != nullptr)
		{
			ptr->ref += 1;
		}
	}
	my_shared_ptr(my_shared_ptr&& _Y) :ptr(_Y.ptr)
	{
		_Y.ptr = nullptr;
	}
	operator bool() const { return ptr != nullptr; }

	my_shared_ptr& operator=(const my_shared_ptr& _Y)
	{
		if (this == &_Y || this->ptr == _Y.ptr) return *this;
		if (ptr != NULL && --ptr->ref == 0)
		{
			mDeletor(ptr->mptr);
			delete ptr;
		}
		ptr = _Y.ptr;
		if (ptr != nullptr)
		{
			ptr->ref += 1;
		}
		return *this;
	}
	my_shared_ptr& operator=(my_shared_ptr&& _Y)
	{
		if (this == &_Y) return *this;
		if (this->ptr == _Y.ptr && this->ptr != nullptr && _Y.ptr != nullptr)
		{
			this->ptr->ref -= 1;
			_Y.ptr = nullptr;
			return *this;
		}
		if (this->ptr != nullptr && --ptr->ref == 0)
		{
			mDeletor(ptr->mptr);
			delete ptr;
		}
		ptr = _Y.ptr;
		_Y.ptr = nullptr;
		return *this;
	}
	void reset(_Ty* p = nullptr)
	{
		if (this->ptr != nullptr && --this->ptr->ref == 0)
		{
			mDeletor(ptr->mptr);
			delete ptr;
		}
		ptr = new RefCnt<_Ty>(p);
	}
	~my_shared_ptr()
	{
		if (this->ptr != nullptr && --this->ptr->ref == 0)
		{
			mDeletor(ptr->mptr);
			delete ptr;
		}
		ptr = nullptr;
	}
	_Ty* get() const { return ptr->mptr; }
	_Ty& operator*() const
	{
		return *get();
	}
	_Ty* operator->() const
	{
		return get();
	}

	size_t use_count() const
	{
		if (this->ptr == nullptr) return 0;
		return this->ptr->ref;
	}
	void swap(my_shared_ptr& r)
	{
		std::swap(this->ptr, r.ptr);
	}

	_Ty& operator[](const int idx) const
	{
		return ptr->mptr[idx];
	}

private:
	RefCnt<_Ty>* ptr;
	_Dx mDeletor;
};
```

强智能指针加入引用计数，可以安全使用拷贝构造函数以及赋值运算符重载函数，但是仍应该避免使用一个裸指针初始化或者赋值多个智能指针，这样将重复创建引用计数结构，引用计数不能正确处理资源的释放导致程序崩溃。

与强智能指针相关的是**交叉引用问题**，有如下两个类：

```c++
class A
{
public:
	A()
	{
		cout << "A()" << endl;
	}
	~A()
	{
		cout << "~A()" << endl;
	}
	shared_ptr<B> _ptr_b;
};

class B
{
public:
	B()
	{
		cout << "B()" << endl;
	}
	~B()
	{
		cout << "~B()" << endl;
	}
	shared_ptr<A> _ptr_a;
};
```

主函数中进行如下调用：

```c++
int main()
{
	shared_ptr<A> a_p(new A());
	shared_ptr<B> b_p(new B());
	a_p->_ptr_b = b_p;
	b_p->_ptr_a = a_p;
    return 0;
}
```

最后输出结果只有类A和类B的构造函数调用，没有进行析构。我们可以看到智能指针 a_p 指向类A，智能指针 b_p 指向类B，同时类A中的成员 _ptr_b 指向类B，类B中的成员 _ptr_a 指向类A，所以根据强智能指针引用计数的特性，类A和类B在分别被引用了两次，所以当调用析构函数的时候类A和类B都不能完全析构。解决这一情况的方法是将类A和类B中成员变量从强智能指针改为弱智能指针。

### weak_ptr

弱智能指针不能直接使用，它必须借助强智能指针初始化；也不能直接解引用，需要将弱智能指针转换为强智能指针后才可以解引用；弱智能指针的使用方法如下所示：

```c++
shared_ptr<int> s_p(new int(10));
weak_ptr<int> w_p(s_p);// 通过强智能指针初始化弱智能指针

cout << *s_p << endl;
cout << *(w_p.lock()) << endl;// 通过lock()方法转换成强智能指针再进行解引用
```

弱智能指针与强智能指针配合使用，所以将强智能指针的源码实现进行了改进，用以适应弱智能指针。以下是改进后的强智能指针和弱引用指针的源码实现：

```c++
template<class _Ty>
class MyDeletor
{
public:
	MyDeletor() = default;
	void operator()(_Ty* ptr) const
	{
		if (ptr != nullptr)
		{
			delete ptr;
		}
	}
};
template<class _Ty>
class MyDeletor<_Ty[]>
{
public:
	MyDeletor() = default;
	void operator()(_Ty* ptr) const
	{
		if (ptr != nullptr)
		{
			delete[]ptr;
		}
	}
};

template<class _Ty>
class RefCnt
{
public:
	_Ty* _Ptr;               
	std::atomic_int _Uses;   // shared_ptr引用计数
	std::atomic_int _Weaks;  // weak_ptr引用计数

public:
	RefCnt(_Ty* p) :_Ptr(p), _Uses(1), _Weaks(1) {}
	~RefCnt() {}
	void _Incref() { _Uses += 1; }
	void _Incwref() { _Weaks += 1; }
};
template<class _Ty> class my_weak_ptr;


template<class _Ty, class _Dx = MyDeletor<_Ty> >
class my_shared_ptr
{
private:
	_Ty* _Ptr;
	RefCnt<_Ty>* _Rep;
	_Dx _mDeletor;
public:
	my_shared_ptr(_Ty* p = nullptr) :_Ptr(nullptr), _Rep(nullptr)
	{
		if (p != nullptr)
		{
			_Ptr = p;
			_Rep = new RefCnt<_Ty>(p);
		}
	}
	my_shared_ptr(const my_shared_ptr& _Y) :_Ptr(_Y._Ptr), _Rep(_Y._Rep)
	{
		if (_Rep != nullptr)
		{
			_Rep->_Incref();
		}
	}
	my_shared_ptr(my_shared_ptr&& _Y) :_Ptr(_Y._Ptr), _Rep(_Y._Rep)
	{
		_Y._Ptr = nullptr;
		_Y._Rep = nullptr;
	}
	my_shared_ptr& operator=(const my_shared_ptr& r)
	{
		if (this == &r || this->_Ptr == r._Ptr) return *this;
		if (_Ptr != nullptr && --_Rep->_Uses == 0)
		{
			_mDeletor(_Ptr);
			if (--_Rep->_Weaks == 0)
			{
				delete _Rep;
			}
		}
		_Ptr = r._Ptr;
		_Rep = r._Rep;
		if (_Ptr != nullptr)
		{
			_Rep->_Incref();
		}
		return *this;
	}
	my_shared_ptr& operator=(my_shared_ptr&& _Y)
	{
		if (this == &_Y) return *this;
		if (_Ptr != nullptr && _Y._Ptr != nullptr && _Ptr == _Y._Ptr)
		{
			this->_Rep->_Uses -= 1;
			_Y._Ptr = nullptr;
			_Y._Rep = nullptr;
			return *this;
		}

		if (_Ptr != nullptr && --_Rep->_Uses == 0)
		{
			_mDeletor(_Ptr);
			if (--_Rep->_Weaks == 0)
			{
				delete _Rep;
			}
		}
		_Ptr = _Y._Ptr;
		_Rep = _Y._Rep;
		_Y._Ptr = nullptr;
		_Y._Rep = nullptr;
		return *this;
	}
	~my_shared_ptr()
	{
		if (_Rep != nullptr && --_Rep->_Uses == 0)
		{
			_mDeletor(_Ptr);
			if (--_Rep->_Weaks == 0)
			{
				delete _Rep;
			}
		}
		_Ptr = nullptr;
		_Rep = nullptr;
	}

	_Ty* get() const { return _Ptr; }
	_Ty& operator*() const { return *get(); }
	_Ty* operator->() const { return get(); }
	size_t use_count() const
	{
		if (_Rep == nullptr) return 0;
		return _Rep->_Uses;
	}
	void swap(my_shared_ptr& r)
	{
		std::swap(_Ptr, r._Ptr);
		std::swap(_Rep, r._ref);
	}
	operator bool() const { return _Ptr != nullptr; }

	template<class _Ty>
	friend class my_weak_ptr;
};


template<class _Ty>
class my_weak_ptr
{
private:
	RefCnt<_Ty>* _Rep;
public:
	my_weak_ptr() :_Rep(nullptr) {}
	my_weak_ptr(const my_shared_ptr<_Ty>& other) :_Rep(other._Rep)
	{
		if (_Rep != nullptr)
		{
			_Rep->_Incwref();
		}
	}
	my_weak_ptr(const my_weak_ptr& other) :_Rep(other._Rep)
	{
		if (_Rep != nullptr)
		{
			_Rep->_Incwref();
		}
	}
	my_weak_ptr(my_weak_ptr&& other) :_Rep(other._Rep)
	{
		other._Rep = nullptr;
	}
	my_weak_ptr& operator=(const my_weak_ptr& other)
	{
		if (this == &other || this->_Rep == other._Rep) return *this;
		if (_Rep != nullptr && --_Rep->_Weaks == 0)
		{
			delete _Rep;
		}
		_Rep = other->_Rep;
		if (_Rep != nullptr)
		{
			_Rep->_Incwref();
		}
		return *this;
	}
	my_weak_ptr& operator=(my_weak_ptr&& other)
	{
		if (this == &other) return *this;
		if (this->_Rep != nullptr && other._Rep != nullptr && _Rep == other._Rep)
		{
			this->_Rep->_Weaks -= 1;
			other._Rep = nullptr;
			return *this;
		}
		if (_Rep != nullptr && --_Rep->_Weaks == 0)
		{
			delete _Rep;
		}
		_Rep = other._Rep;
		other._Rep = nullptr;
		return *this;
	}
	my_weak_ptr& operator=(const my_shared_ptr<_Ty>& other)
	{
		if (_Rep != nullptr && --_Rep->_Weaks == 0)
		{
			delete _Rep;
		}
		_Rep = other->_Rep;
		if (_Rep != nullptr)
		{
			_Rep->_Incwref();
		}
		return *this;
	}
	my_weak_ptr& operator=(my_shared_ptr<_Ty>&& other) = delete;

	~my_weak_ptr()
	{
		if (_Rep != nullptr && --_Rep->_Weaks == 0)
		{
			delete _Rep;
		}
		_Rep = nullptr;
	}
	bool expired() const// 资源是否被释放
	{
		return this->_Rep->_Uses == 0;
	}
	my_shared_ptr<_Ty> lock() const
	{
		my_shared_ptr<_Ty> _Ret;
		_Ret._Ptr = _Rep->_Ptr;
		_Ret._Rep = _Rep;
		_Ret._Rep->_Incref();
		return _Ret;
	}
};
```

弱智能指针包含一个指向引用计数结构的指针，并不会对资源进行直接的管理，强智能指针负责对象资源的释放。使用弱智能指针可以解决交叉引用的问题：将类 A 和类 B 中的成员变量替换成弱引用指针，交叉引用只会增加弱智能指针的计数，而资源是否释放根据强智能指针计数判断，这样对象资源可以安全释放，而引用计数结构通过弱智能指针计数值释放。

## C++ I/O

C方式 I/O，C语言装入的输入输出格式。

```c++
#include<stdio.h>
int main()
{
	char c_fgetc = fgetc(stdin);// 从流中读取一个字符
	char c_getc = getc(stdin);// 从流中读取一个字符
	char c_getchar = getchar();// 从标准输入流中读取一个字符

	char str_fgets[10] = { 0 };
	fgets(str_fgets, sizeof(str_fgets), stdin);// 从流中读取长度为n-1的字符串，转储到str中
	char str_gets_s[10] = { 0 };
	gets_s(str_gets_s, sizeof(str_gets_s));// 从标准输入流中读取长度为n-1的字符串，转储到str中

	fputc('c', stdout);// 向流中写入一个字符
	putc('c', stdout);// 向流中写入一个字符

	fputs("hello string", stdout);// 向流中写入字符串
	puts("hello string");// 向标准输出流中写入字符串
	putchar('c');// 向标准输出流写入一个字符

	return 0;
}
```

C++方式 I/O，采用C++风格的输入输出格式。

```c++
#include<iostream>
int main()
{
	char str_a[5];
	for (int i = 0; i < 5; i++)
	{
		cin >> str_a[i];// 不认空格，不认换行
	}
	cout << str_a << endl;


	char str_b[5];
	cin >> str_b;// 见到空格，换行结束读入，未读入的字符残留在输入流
	cout << str_b << endl;

	char str_c[5];
	for (int i = 0; i < 5; i++)
	{
		cin.get(str_c[i]);// 认空格，换行结束读入
	}
	cout << str_c << endl;

	char str_d[5];
	cin.get(str_d, 5);// 认空格，换行结束读入
	cout << str_d;

	char str_e[10];
	cin.getline(str_e, 10);// 认空格，默认换行结束读入
	cin.getline(str_e, 10, '#');// 认空格，认换行，自定义结束符
	cout << str_e;
}
```

字符串流方式读入字符串，采用特殊方式读入字符串，并且可以根据类型将读入的字符串进行类型转换。

```c++
#include<iostream>
#include<string>
#include<sstream>
int main()
{
	// 字符串流方案1举例
	string line;
	while (getline(cin, line))
	{
		stringstream ss(line);
		int sum = 0, num;
		while (ss >> num) sum += num;// string -> int
		cout << sum << endl;
	}

    // 字符串流方案2举例
	string line;
	while (getline(cin, line))
	{
		stringstream ss(line);
		string word;
		while (ss >> word)cout << word << endl;
	}
	return 0;
}
```

## C++风格指针以及函数封装器

不一定所有的空指针都与零等价，反例为当定义一个指向类公有成员的指针时，空指针此时不为零。

```c++
#include<iostream>
#include<functional>
int my_pow(int n, int x)
{
	for (int i = 0;i < x;++i)
	{
		n = n * 10;
	}
	return n;
}
class A
{
public:
	int count;
	int num;
public:
	A(int c, int n) :count(c), num(n) {}
	void fun()
	{
		cout << count << " ";
		cout << __FUNCTION__ << endl;// 宏，打印函数名
	}
	static void add(int a)
	{
		cout << a + a << " ";
		cout << __FUNCTION__ << endl;
	}
	void operator()(int x)
	{
		cout << x << " ";
		cout << __FUNCTION__ << endl;
	}
};
int main()
{
	int(*p)(int, int) = nullptr;// 普通函数指针
	p = my_pow;
	cout << p(2, 3) << endl;

	int A::* num_ptr = nullptr;// 类成员指针，偏移量
	num_ptr = &A::num;
	int A::* count_ptr = nullptr;// 类成员指针，偏移量
	count_ptr = &A::count;

	A a(10, 20);// 实例化对象.*运算符取值
	cout << a.*num_ptr << endl;
	cout << a.*count_ptr << endl;

	void (A:: * obj)() = nullptr;// 类方法函数指针
	obj = &A::fun;
	(a.*obj)();

	function<void(int)> fr1 = a;// 仿函数
	fr1(10);
	function<void(int)> fr2 = &A::add;// 静态函数
	fr2(10);
	function<void(A&)> fr3 = &A::fun;// 普通成员函数
	fr3(a);
}
```

