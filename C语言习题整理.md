# 字符串相关代码训练

## 字符串长度

```c
int my_strlen1(const char* str) {
	assert(str != nullptr);
	int i = 0;
	while (str[i] != '\0') {
		i++;
	}
	return i;
}
int my_strlen2(const char* str) {
	assert(str != nullptr);
	const char* sp = str;
	while (*sp != '\0') {
		sp++;
	}
	return sp - str;
}
```
两个函数都可以正确计算并返回字符串的长度，其中my_strlen2函数通过指针相减的方式获取两个指针之间元素的个数。

## 字符串拷贝

```c
char* my_strcpy(char* dest, const char* src) {
	assert(dest != nullptr && src != nullptr);
	char* cp = dest;
	while (*src != '\0') {
		*cp = *src;
		cp++;
		src++;
	}
	*cp = '\0';
	return dest;
}
```
需要注意的是，定义指针cp是有意义的。考虑到需要将指针作为函数的返回值，指针dest需要始终指向字符串的起始位置，所以使用cp移动遍历整个字符串，执行拷贝操作。

## 字符串连接

```c
char* my_strcat(char* dest, const char* src) {
	assert(dest != nullptr && src != nullptr);
	char* cp = dest;
	while (*cp != '\0') {
		cp++;
	}
	while (*src != '\0') {
		*cp++ = *src++;
	}
	*cp = '\0';
	return dest;
}
```
该函数的作用是将src指向的字符串连接到dest指向的字符串后面，首先将指针cp定位到dest字符串的末尾，然后将src字符串中的内容拷贝进dest字符串。

## 字符串比较

```c
int my_strcmp(const char* fs, const char* cs) {
	assert(fs != nullptr && cs != nullptr);
	while (*fs != '\0' && *cs != '\0') {
		if (*fs != *cs) {
			break;
		}
		fs++;
		cs++;
	}
	return *fs - *cs;
}
```
比较字符串fs和cs，也就是将fs的每个字符和cs的每个字符的ascll码进行比较：如果fs > cs返回一个大于零的值；如果fs < cs返回一个小于零的值；如果fs = cs返回零值。

> 总结：注意字符串末尾存在的‘\0’，即便不显示出来，但是每一个字符串末尾都会有这一字符。所以在对字符串进行拷贝等操作时，不能忘记将该字符也进行操作。
> 对于指针来说，p != '\0'这种写法理论上存在问题，但是编译器不会报错，而且程序无法正常运行，应将其改成*p != '\0'或者是p != nullptr。
> 如果想让一个数据在定义好后不再被更改，使用常量指针可以防止通过指针意外地改变该数据。



# 数组的平移

## 循环方法

1.数组向左平移一位

```c
void Left_Move_Ar(int* br, int n) {
	assert(br != nullptr && n > 1);
	int tmp = br[0];
	for (int i = 0;i < n - 1;i++) {
		br[i] = br[i + 1];
	}
	br[n - 1] = tmp;
}
```
assert()称为断言，是assert.h头文件中的函数，只有当括号中的条件成立时，程序才会继续执行下面的操作。需要注意的是，断言机制只有在debug环境下才能执行。首先将数组中第一个元素存入临时变量中，然后一次左移数组中每一个元素，最后将临时变量中存储的数组第一个元素放到数组末尾，完成了一次数组的左移。

2.数组向右平移一位

```c
void Right_Move_Ar(int* br, int n) {
	assert(br != nullptr && n > 1);
	int tmp = br[n - 1];
	for (int i = n - 1;i > 0;i--) {
		br[i] = br[i - 1];
	}
	br[0] = tmp;
}
```
和第一个函数原理相似，但是要注意循环条件的设置。

3.数组向左平移k位

```c
void Left_Move_Ar_k(int* br, int n, int k) {
	assert(br != nullptr);
	k = k % n;
	if (k >= 0) {
		while (k--) {
			Left_Move_Ar(br, n);
		}
	}
	else {
		while (k++) {
			Right_Move_Ar(br, n);
		}
	}
}
```
由于数组的性质，平移k位和平移k+n位或者k+2n位本质上都是平移k位，所以对k取余数，这样可以省去不必要的移动。如果传入的k值大于零说明是左移，小于零说明是右移，调用第一个和第二个函数。

4.数组向右平移k位

```c
void Right_Move_Ar_k(int* br, int n, int k) {
	assert(br != nullptr);
	Left_Move_Ar_k(br, n, -k);
}
```
右移k位和左移k位原理相同，只是符号相反，所以我们将传进来的k值取反，然后调用第三个函数即可。

## 颠倒数列

```c
void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
void ReverseArray(int* br, int left, int right) {
	while (left < right) {
		swap(&br[left++], &br[right--]);
	}
}
void Left_Move_Ar_k(int* br, int n, int k) {
	k = k % n;
	ReverseArray(br, 0, k - 1);
	ReverseArray(br, k, n - 1);
	ReverseArray(br, 0, n - 1);
}
void Right_Move_Ar_k(int* br, int n, int k) {
	k = n - (k % n);
	ReverseArray(br, 0, k - 1);
	ReverseArray(br, k, n - 1);
	ReverseArray(br, 0, n - 1);
}
void Print_Ar(int* br, int n) {
	assert(br != nullptr && n > 1);
	for (int i = 0;i < n;i++) {
		printf("%4d ", br[i]);
		if ((i + 1) % 5 == 0)printf("\n");
	}
	printf("\n");
}
```
翻转函数中，left和right都代表数组下标。注意循环结束的条件是left < right而不是left != right。

## 结构体

```c
#define ARSIZE 10
struct MoveArray {
	int data[ARSIZE];
	int curpos;
	int maxsize;
};
void Init_Ar(struct MoveArray* par) {
	par->maxsize = ARSIZE;
	par->curpos = 0;
	int br[ARSIZE] = { 12,23,34,45,56,67,78,89,90,100 };
	for (int i = 0;i < par->maxsize;i++) {
		par->data[i] = br[i];
	}
}
void Left_Move_ar_k(struct MoveArray* par, int k) {
	k = k % par->maxsize;
	par->curpos = k;
}
void Right_Move_ar_k(struct MoveArray* par, int k) {
	k = k % par->maxsize;
	par->curpos = par->maxsize - k;
}
int GetElem(struct MoveArray ar, int index) {
	return ar.data[(ar.curpos + index) % ar.maxsize];
}
void Print_Ar(struct MoveArray ar) {
	for (int i = 0;i < ar.maxsize;i++) {
		printf("%d ", GetElem(ar, i));
	}
	printf("\n");
}
```
前两套方案会改变数组元素的值，这一套方案不会。采用MoveArray结构体实现，定义了一个定长数组data，和指示数组当前起始位置的curpos，还有最大长度maxsize。在GetElem函数接受的参数中，index表示数组的下标。数组各个元素的位置不会被更改，为了实现左移和右移的效果，改变的是数组当前的起始位置，这样数组元素读取出来的顺序就会改变。

> 总结：循环的开始和结束条件是关键，其次要清楚每一次循环程序所做的工作是什么。
> for循环与while循环很多情况下可以是可以等价互换的，所以选择循环结构时不用只拘泥于一种结构。
> 思维要开阔，如果程序有多种解法，尽量选择更省时省力的方法，有时运用巧妙的逻辑或是简单的数学知识可以将一些代码化简。
> 程序并不是越简洁越好，有时候过于简洁的程序可读性会变差，让人难以理解，所以使程序简洁明了的同时又兼具可读性是开发者始终要去追求的事情。

# 判断闰年

```c
bool Is_LeapYear(int year)
{
	return (year % 4 == 0 && year % 100 != 0 || year % 400 == 0);
}
```
函数接受一个整型数，即用户输入的年份，返回一个布尔类型常量。判断闰年的方法是看输入的年份是否能被4整除或者被400整除，若能被4整除且不能被100整除就是闰年；若能被400整除就是闰年。

# 二分查找

又称折半查找，优点是比较次数少，查找速度快，平均性能好；缺点是要求待查找表是有序表，而且插入删除困难。因此，二分查找适用于不经常变动的有序列表。首先，假设表中元素按升序排列，将表中间的元素与待查找元素进行比较，如果两者相等，则查找成功；否则利用中间位置元素将表分成前后两个子表，如果表中间元素大于待查找元素，则在前一个子表中查找，否则在后一个子表中查找。重复以上过程，直到找到满足条件的记录，使查找成功，或直到子表不存在为止，这时查找失败。

## 循环方法

```c
int findvalue(int* br, int n, int val) {//二分查找，循环方法
	assert(br != nullptr);
	int pos = -1;
	int left = 0, right = n - 1;
	while (left <= right) {
		int mid = (right - left) / 2 + left;
		if (val < br[mid]) {
			right = mid - 1;
		}
		else if (val > br[mid]) {
			left = mid + 1;
		}
		else {
			pos = mid;
			break;
		}
	}
	return pos;
}
```
该函数接收一个有序数组，如果查找成功，返回元素所在数组下标；如果查找失败，返回-1。需要注意的是，关于变量mid的定义，比较以下两种形式：

```c
int mid = (right - left) / 2 + left;//1
int mid = (right + left) / 2;//2
```
这两种算法计算出表中间元素的位置都是正确的，但是当表中元素非常多时，数组下标很大，第二种算法在相加时可能会产生溢出问题。所以使用第一种写法更安全，使程序更加健壮。

## 递归方法

```c
int search(int* br, int left, int right, int val) {
	int pos = -1;
	if (left <= right) {
		int mid = (right - left) / 2 + left;
		if (br[mid] > val)
			pos = search(br, left, mid - 1, val);
		else if (br[mid] < val)
			pos = search(br, mid + 1, right, val);
		else pos = mid;
	}
	return pos;
}
int searchvalue(int* br, int n, int val) {//二分查找，递归方法
	assert(br != nullptr);
	return search(br, 0, n - 1, val);
}
```
程序思路和二分查找的定义一样，在左下标小于等于右下标的前提下，如果中间元素的值大于待查找值，那么以中间元素为界，在前一个子表中再一次调用二分查找，否则在后一个子表中使用二分查找，以此类推。直到找到待查找元素，返回下标值，查找失败则返回-1。

> 总结：
>
> 递归是分治策略的一种体现，面对规模大的问题，将其拆分为数个规模较小的相同问题解决，是编程中一种重要的思想。
> 递归和循环是可以相互转换的。相较于循环程序，递归程序的主要特点是简洁，通常没有循环程序那么长的代码叙述。同循环程序一样，在编写递归函数的时候，要考虑时间复杂度的问题，同时，编写递归函数还要防止出现无限递归的情况，导致栈溢出。
> 编写程序要不仅仅是将代码写对，还要将代码写好。考虑带入临界值、特殊值是否可以正常运行，面对不合法的参数应该如何处理等等，查漏补缺是程序编写完成后一个十分关键的步骤。

# 输入年月，返回该月的天数

## 第一套方案

```c
int Get_YM_Day(int year, int month) {
	static const int days[13] = { 29,31,28,31,30,31,30,31,31,30,31,30,31 };
	if (month == 2 && Is_LeapYear(year)) 
	{
		month = 0;
	}
	return days[month];
}
```
该函数接受两个整型数，一个是年份，一个是月份，返回一个整型值，表示天数。采用查表法，定义一个数组，里面存储12个月份的天数。需要注意的是，由于有闰年的存在，二月份存在两个值，我们把闰年时二月份的天数存入数组下标为零的位置。判断条件为当月份是二月份且年份为闰年时，将数组下标设置为2并返回该数组下标元素；若是其他月份就直接根据用户给出的月份定位到数组对应下标元素并返回即可。

## 第二套方案

```c
int Get_YM_Day(int year, int month) {
	int day = 0;
	int flag = month;
		switch (flag) {

			case 1:case 3:case 5:case 7:case 8:case 10:case 12:
				day = 31;
				break;
			case 4:case 6:case 9:case 11:
				day = 30;
				break;
			default:
				if (Is_LeapYear(year)) {
					day = 29;
					break;
				}
				else {
					day = 28;
					break;
				}
		}
		return day;
}
```
和第一套方案不同的是，采用switch语句，对输入的月份作为判断条件，根据不同的月份进入相应的case当中，为返回值赋值。注意，对于特殊的2月来说，利用第一个函数判断是否是闰年。

# 输入年月日，返回是这一年的第几天

## 第一套方案

```c
int Get_YMD_Total(int year,int month,int day) {
	int sum = 0;
	if (year < 1)return YEAR_ERROR;
	if (month < 1 || month > 12)return MONTH_ERROR;
	if (day < 1 || day > Get_YM_Day(year, month))return DAY_ERROR;
	for (int i = 1;i < month; i++) 
	{
		sum = sum + Get_YM_Day(year, i);
	}
	sum = sum + day;
	return sum;
}
```
该函数接受三个整型数，一个是年份，一个是月份，一个是天数，返回一个整型值，表示天数。计算x年y月z日是一年中的第几天，应该累加前y-1个月每个月的天数，加完之后加上z就可以算出最终结果。需要注意的一点是，由于需要知道每一个月的天数然后累加，我们需要使用到第二个函数。除此之外，加入错误判断语句，针对用户输入年、月、日不合法的情况，返回对应的错误代码。

## 第二套方案

```c
int Get_YMD_Total(int year, int month, int day) {
	int total[13] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };

	if (year < 1)return YEAR_ERROR;
	if (month < 1 || month > 12)return MONTH_ERROR;
	if (day < 1 || day > Get_YM_Day(year, month))return DAY_ERROR;

	if (month > 2 && Is_LeapYear(year)) {
		++day;
	}

	return total[month - 1] + day;
}
```
采用查表法。注意区分闰年和平年的区别，也就是用户在查询2月之后的某天是这一年的第几天时，闰年需要在平年的基础上加一，因为闰年的2月比平年的2月多一天。

> 总结：
> 三个看似独立的编程练习，实际上是层层递进的关系。这告诉我们一个重要的编程思想：处理一个大的问题，常常将其拆解成一个个小的问题然后分别解决。在实际工作当中我们直接面对的可能是第三个程序那样的问题，这类问题一般不能被我们所学的现有知识直接解答，那么如何解决这个问题？就是将它一层层分解，得出问题的零元，也就是得到我们熟悉的知识点上去，然后在反向套回去。一个复杂的问题就迎刃而解了。

***更多的代码练习***

*编程练习：二维数组*

有一个元素有序的二维数组，现在要编写一个查询函数，查询元素的对应下标。

```c
int FindValue1(int(*br)[5], int row, int col, int val) {
	int pos = -1;
	int i = 0;
	while (i<row && val>br[i][col - 1]) {
		i++;
	}
	if (i == row)return pos;
	int j = col - 1;
	while (j >= 0 && val < br[i][j]) {
		j--;
	}
	if (j >= 0 && val == br[i][j])
	{
		pos = i * col + j;
	}
	return pos;
}
```
由于二维数组是有序的，每一行的最后一个元素都小于下一行的第一个元素，所以采用分块查找，首先定位待查找应该在第几行，然后再定位是第几列，查找失败返回-1。

```c
int FindValue2(int(*s)[5], int row, int col, int val) {
	int* p = (int*)s;
	int num = row * col;
	int left = 0, right = num - 1, pos = -1;
	while (left <= right) {
		int mid = (right - left) / 2 + left;
		if (val < p[mid]) {
			right = mid - 1;
		}
		else if (val > p[mid]) {
			left = left + 1;
		}
		else {
			pos = mid;
			break;
		}
	}
	return pos;
}
```
根据二维数组的性质，可以将其转化成一个大的一维数组使用，又因为数组元素有序，所以可以采用二分查找，这种方式的查询次数要比第一种方式的少，效率更高。

*位运算举例：求二进制1的个数*

```c
//方案一：
int Get1Bit(int x) {
	int num = 0;
	while(x != 0)
	{
		if ((x & 0x01) == 1)
		{
			num++;
		}
		x = x >> 1;
	}
	return num;
}
//方案二：
int Get1Bit(int x) {
	int sum = 0, tmp = 0;
	int ar[] = { 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4 };
	int a = 0x0f;
	while(x != 0) {
		tmp = x & a;
		sum = sum + ar[tmp];
		x = x >> 4;
	}
	return sum;
}
//方案三：
int Get1Bit(int x) {
	int sum = 0;
	while (x != 0) 
	{
		sum = sum + "\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4"[x & 0x0f];
		x = x >> 4;
	}
	return sum;
}
//使用了查表方式，根据位与后的结果寻找对应下标取值，转义字符将字符转换成了八进制的数值。
```
*编程练习：找出数组中的最大值和次大值*

```c
void Print2Max(int* ar, int n) {
	assert(ar != nullptr && n >= 0);
	int max1 = ar[0] > ar[1] ? ar[0] : ar[1];
	int max2 = ar[0] < ar[1] ? ar[0] : ar[1];
	for (int i = 2;i < n;i++) {
		if (ar[i] > max1) {
			max2 = max1;
			max1 = ar[i];
		}
		else if (ar[i] > max2) {
			max2 = ar[i];
		}
	}
	printf("max1:%d max2:%d\n", max1, max2);
}
```
*状态方案例题：计算单词个数*

```c
#define BEGIN		1
#define IN_WORD		2
#define OUT_WORD	3
#define END			4
int GetWordNum(const char* str) {
	assert(str != nullptr);
	int num = 0;
	int tag = BEGIN;
	for (const char* p = str;*p != '\0';p++) {
		switch (tag) {
		case BEGIN:
			if (isalpha(*p)) {
				tag = IN_WORD;
			}
			else {
				tag = OUT_WORD;
			}
			break;
		case IN_WORD:
			if (!(isalpha(*p) || *p == '\'' || *p == '-')) {
				tag = OUT_WORD;
				num += 1;
			}
			break;
		case OUT_WORD:
			if (isalpha(*p)) {
				tag = IN_WORD;
			}
			break;
		}
	}
	if (tag == IN_WORD) {
		num += 1;
	}
	return num;
}
```
设置一个状态标记tag，一开始将其置为“初始”，然后进入条件判断：如果是字母，将当前状态置为“单词内”，否则置为“单词外”；每当状态从“单词内”变成“单词外”时，说明扫描了一个单词，将计数器加一。一直循环往复，直到指针遍历完整个字符串为止。需要注意的是，由于只有当状态从“单词内”变成“单词外”是才会计数，所以如果最后一个单词后面没有非字母的话，程序将会少记录一个单词。解决方法是在循环结束后查看标记的状态，如果标记的状态是“单词内”，说明字符串的末尾是字母，应该让计数器再加一，补上少记录的一个单词。

# 寻找最大值

```c
int findmax_2(int a, int b) {
	int max = a > b ? a : b;
	return max;
}
int findmax_3(int a, int b, int c) {
	int max = a > b ? a : b;
	max = max > c ? max : c;
	return max;
}
int findmid_3(int a, int b, int c) {
	int min = a > b ? b : a;
	min = min > c ? c : min;

	int max = a > b ? a : b;
	max = max > c ? max : c;

	int mid = (a + b + c) - (max + min);
	return mid;
}
```
# 指针与数组例题

```c
int main() {
	int ar[5] = { 12,23,34,45,56 };
	int x = 0, y = 0;
	int* p = ar;

	x = *++p;		//23
	y = *p; 		//23
	printf("%d %d\n", x, y);

	x = ++ * p;	//24
	y = *p;		//24
	printf("%d %d\n", x, y);

	x = *p++;	//24 ==> x=*p; p++;
	y = *p;		//34
	printf("%d %d\n", x, y);

	x = (*p)++;	//34
	y = *p;		//35
	printf("%d %d\n", x, y);

	return 0;
}
```
p是指向数组首元素的指针变量，++p移动p指针指向数组中第二个元素的位置，然后*运算符取出元素的值，所以x = 23，y = 23。
*p取出元素的值，此时p指向数组第二个元素的位置，然后前置自增运算符使第二个位置元素的值自增1，所以x = 24，y = 24。
首先执行x = *p，取出此时p所指向元素的值，x = 24；然后执行p++，p是指针变量，自增将其移动到数组中第三个元素的位置，所以y = 34。
*p首先取出指针当前指向元素的值，由于是后置自增运算符，所以x = 34，y=35。

# 循环程序的强化训练

## 1.冒泡排序及优化

```c
void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
void Bubblesort(int* ar, int n)
{
	for (int i = 0;i < n - 1;i++)
	{	
		bool flag = true;
		for (int j = 0;j < n - 1 - i;j++)
		{
			if (ar[j] > ar[j + 1])
			{
				swap(&ar[j], &ar[j + 1]);
				flag = false;
			}
		}
		if (flag)break;
	}
}
```
根据冒泡排序的性质，当一趟比较当中如果没有交换任何一组元素，那么此时这个数组是有序的，不需要再进行之后的比较了。由此在每一趟排序之前定义一个布尔类型变量，将其设置为真，如果至少发生了一次交换，就说明这一趟比较交换了元素，将其设置为假。这样如果有一次循环没有进行交换那么这个布尔类型的变量就不会被改变成假，所以加入一个if条件判断就可以知道当布尔类型变量为真时，跳出循环结束比较。

## 2.查找数组元素，返回下标

```c
int FindValue(int* br, int n, int val) {
	int pos = -1;
	for (int i = 0;i < n;i++) {
		if (br[i] == val) {
			pos = i;
			break;
		}
	}
	return pos;
}
```
传入val变量，若找到该元素，返回其下标，若没有找到，返回-1。

## 3.初始化数组，要求在数组中填充不重复的随机值

```c
void Init_Ar(int* br, int n) {
	int i = 0;
	while (i < n) {
		int tmp = rand() % RANDSIZE + 1;
		if (FindValue(br, i, tmp) == -1) {
			br[i++] = tmp;
		}
	}
}
```
第一套方案，使用查找函数。每次向数组填充进新的随机值时，在数组已填充部分查找新填充的值，如果查找不到，说明是一个不重复的随即值，可以填充进入数组；若查找成功，说明新填充的随机数在之前已经生成过，不能填充进数组。

```c
void Init_Ar(int* br, int n) {
	int table[RANDSIZE + 1] = {};
	int i = 0;
	while (i < n) {
		int tmp = rand() % RANDSIZE + 1;
		if (table[tmp] == 0) {
			table[tmp] = 1;
			br[i++] = tmp;
		}
	}
}
```
第二套方案，使用查表法。初始化一个数组长度的表，将其初始化为零。每当一个随机值填充进数组前，我们找到随机数在表中的位置，判断是否是零，若是零，则说明允许填充，将元素送进数组，然后将表中这个位置的值由零改为一；反之，一个随机值产生，在表中对应位置不是零，那么说明这个数已经存在于数组中，不能再填充进数组了。通过这种方式我们可以高效的完成数组的初始化。

# 无类型指针应用：泛型编程

```c
void my_memcpy(void* dest, const void* src, int size) {
	assert(dest != nullptr && src != nullptr);
	char* dp = (char*)dest;
	const char* sp = (const char*)src;
	for (int i = 0;i < size;i++) {
		dp[i] = sp[i];
	}
}
```
函数的功能是将src指针指向的空间中的内容拷贝到dest指针指向的空间当中。由于src看做源操作数，不允许通过指针改变源数据的内容，所以将src定义为常量指针。由于形参是无类型指针，所以无论实参传进来的是什么类型的指针，形参都可以接收并存储。然后我们使用char类型的指针，进行强制类型转换，由于char类型长度是一个字节，所以我们一边移动char类型指针，一边对指针所指向位置的值进行拷贝。我们可以看出，传递进来的参数无论是什么类型，该函数都可以进行拷贝。

```c
void my_setzero(void* dest, int size) {
	char* dp = (char*)dest;
	for (int i = 0;i < size;i++) {
		dp[i] = '\0';
	}
}
```
函数功能是将dest指向的内存中的内容初始化，比如说整型初始化成0，字符型初始化成'\0'。使用无类型指针，然后将无类型指针强转为char*类型指针，通过移动该指针，一个字节一个字节将内容初始化。*

# 递归代码专题

## 1.接收整型数，将它的每一位逆序打印输出

```c
void fac(unsigned int n) {//逆序输出
	if (n != 0) {
		printf("%d ", n % 10);
		fac(n / 10);
	}
}
```
若想正序打印输出，只需要将程序中第三行和第四行交换顺序即可。

## 2.辗转相除法求最大公约数

```c
int gcd(int a, int b) {//辗转相除
	if (b != 0)return gcd(b, a % b);
	else return a;
}
```
## 3.正序打印数组的元素

```c
void Print_Ar(int* br, int n) {//打印数组
	assert(br != nullptr);
	if (n > 0) {
		Print_Ar(br, n - 1);
		printf("%d ", br[n - 1]);
	}
}
```
若想逆序打印数组，将Print_Ar和printf调换位置即可。

## 4.查找数组中的某一个值并返回其下标

```c
int FindValue_rec(int* br, int n, int val) {//寻找数组中的val值并返回其下标
		if (n - 1 >= 0 && br[n - 1] != val) {
			return FindValue_rec(br, n - 1, val);
		}
		else return n - 1;
}
```
## 5.二分查找

```c
int search(int* br, int left, int right, int val) {
	int pos = -1;
	if (left <= right) {
		int mid = (right - left) / 2 + left;
		if (br[mid] > val)
			pos = search(br, left, mid - 1, val);
		else if (br[mid] < val)
			pos = search(br, mid + 1, right, val);
		else pos = mid;
	}
	return pos;
}
int searchvalue(int* br, int n, int val) {//二分查找，递归方法
	assert(br != nullptr);
	return search(br, 0, n - 1, val);
}
```
## 6.斐波那契数列

```c
int fib1(int n) {//斐波那契数列1，递归方法，时间复杂度O(2^n)
	if (n == 1 || n == 2)return 1;
	else return fib1(n - 1) + fib1(n - 2);
}
```
可以画出函数的递归调用图，是一棵二叉树。

```c
int fac(int n, int a, int b) {
	if (n <= 2)return a;
	else return fac(n - 1, a + b, a);
}
int fib2(int n) {//斐波那契数列2，递归方法，时间复杂度O(n)
	int a = 1, b = 1;
	return fac(n, a, b);
}
```
相较于上一种方法，时间复杂度更低。

## 7.求集合的子集

```c
bool add(int* br, int n) {
	if (n <= 0)return false;
	br[n - 1] = br[n - 1] + 1;
	if (br[n - 1] == 2) {
		br[n - 1] = 0;
		return add(br, n - 1);
	}
	else return true;
}

void find_sub(int* ar, const int* br, int n) {
	printf("#\n");
	while (add(ar, n)) {
		for (int j = 0;j < n;j++) {
			if (ar[j])printf("%d ", br[j]);
		}
		printf("\n");
	}
}
```
## 8.青蛙跳台阶

```c
int jumpfloor(int n) {
	if (n == 1)
	{
		return 1;
	}
	else if (n == 2)
	{
		return 2;
	}
	else return jumpfloor(n - 1) + jumpfloor(n - 2);

}
```
青蛙可以一次跳一级台阶，也可以一次跳两级台阶，那么对于n级台阶一共有几种跳法？首先考虑特殊情况：
一级台阶只有一种跳法，二级台阶有两种跳法。三级台阶的情况，可以先选择跳一级，这样剩下的二级就可以套用二级台阶的情况处理；若是选择跳二级，那么剩下一级可以套用一级台阶的情况处理。那么对于n级台阶，跳法可以分解为n-1级台阶的跳法与n-2级台阶的跳法之和，n-1级台阶又可以继续向下分解，n-2同理...，可以看出这是斐波那契数列的一个应用。

# 联合体、sprintf_s和sscanf_s的综合应用：IP地址转换

互联网中，IP地址是一个无符号整型数，不方便记忆，我们采用每八位就将其转换成十进制数的方式，然后加小数点分隔，形成如125.86.5.67的样子。现在有一个IP地址例如是2148205343，如何将其转化为点分十进制，并按照字符串的形式输出出来？如果有一个点分十进制的IP地址，那么如何将其转换回无符号整型数呢？

```c
union UNode {
	unsigned int addr;
	unsigned char sx[4];
};
void Uint_To_Str(char* buffer, int len, unsigned int ip) {
	assert(buffer != nullptr);
	UNode x;
	x.addr = ip;
	sprintf_s(buffer, len, "%d.%d.%d.%d", x.sx[3], x.sx[2], x.sx[1], x.sx[0]);
	printf("%s\n", buffer);
}
```
将无符号整型转化为点分十进制形式相对简单。转换函数接收三个参数，buffer用于保存最终的结果，len是buffer的长度，ip是待转换的IP地址。定义一个联合体变量x，将ip的值赋给x.addr，这样与addr共享空间的数组sx也有了相同的数据。现在数组sx[4]将IP地址分成了四段，每段是八个字节。通过sprintf_s将数组中每个元素输出成整型的格式，并加上小数点，写入到buffer缓冲区中。最后输出缓冲区中的内容即可。

```c
void Str_To_Uint(char* str) {
	assert(str != nullptr);
	UNode x;
	char ch;
	int ir[4];
	int n = sscanf_s(str,"%d.%d.%d.%d%c", &ir[3], &ir[2], &ir[1], &ir[0], &ch);
	if (n == 4) {
		for (int i = 3;i >= 0;i--) {
			if (ir[i] >= 0 && ir[i] <= 255) {
				x.sx[i] = ir[i];
			}
			else {
				printf("取值不合法\n");
				exit(-1);
			}
		}
	}
	else {
		printf("长度不合法\n");
		exit(-1);
	}
	printf("%u\n", x.addr);
}
```
将点分十进制的字符串转换成无符号整型略微复杂，因为要考虑参数不合法的情况。函数接收待转换的字符串，思路是得到字符串中除去小数点外的数字，并将其拼接到一起。定义int类型数组，sscanf_s从流中读取待转换字符串，将每一段数字保存在数组中。在这里，需要判断用户传入的字符串长度是否合法，因为IP地址只有32位，也就是说，字符串只可能有四段数字，多了或者少了都不是合法的IP地址，需要将这种情况发现并且给出错误提示。定义一个变量n负责记录下ssanf_s的返回值，该返回值代表sscanf_s成功接收的参数数目。只有当n等于4的时候，说明sscanf_s接收了4个整形参数，才继续执行下面的程序，否则认为用户给出的IP地址不合法。程序继续进行，获取了四组数字，接下来只需要将其拼接成一个数字即可，通过联合体来实现。定义联合体变量x，将刚刚数组中的数据拷贝进x.sx中。此时需要判断用户给出的每段数字是否合法，因为IP地址每八个二进制位转化为十进制，那么每段数字的大小应该在0~255之间，所以不在这个区间范围的数字都是不合法的参数。如果拷贝成功，那么输出x.addr就是最终IP地址转换后的结果，因为是x联合体，既然sx已经被赋值，那么共享空间的addr也有了相同的数据，而addr是无符号整型，长4个字节，所以输出它就代表着最终的结果。以下是主函数调用的一个用例：

```c
int main() {
	unsigned int ip = 2148205343;
	char buffer[20];
	Uint_To_Str(buffer, 20, ip);
	Str_To_Uint(buffer);
	
	char stra[] = { "128.23.54.65.23" };
	char strb[] = { "240.54.3" };
	char strc[] = { "125.600.23.43" };
	//Str_To_Uint(stra);//长度不合法
	//Str_To_Uint(strb);//长度不合法
	//Str_To_Uint(strc);//取值不合法

	return 0;
}
```
# 上机题目

## 1.最大利润

用一个数组表示股票每天的价格，用户可以在某一天购入这支股票，然后在之后的某一天抛出。如果此时有一支股票每天的价格表示为{1,3,2,7,4}，那么用户应该在第一天购入，在第四天抛出获得最大利润；如果一支股票价格是{3,4,2,6,7,1}，那么用户将无法获得利润。输入一个数组，这个数组代表股票每天的走势，输出用户所能获得的最大利润。

```c
int Get_MaxProfit(int* p, int n) {//最大利润
	assert(p != nullptr);
	int min_tmp = 0;
	int min = p[0];
	for (int i = 1;i < n;i++) {
		if (p[i] < min) {
			min = p[i];
			min_tmp = i;
		}
	}
	if (min_tmp == n - 1) {
		return 0;
	}
	int max = p[min_tmp];
	for (int j = min_tmp + 1;j < n;j++) {
		if (p[j] > max) {
			max = p[j];
		}
	}
	int profit = max - min;
	return profit;
}
```
## 2.字符串计算

获取字符串中单词、空格、数字、其他字符的个数（利用<ctype.h>中的库函数）。

```c
void Statistic(const char* str) {
	//获取字符串中单词、空格、数字、其他字符的个数
	assert(str != nullptr);
	int word = 0, space = 0, num = 0, other = 0;
	for (const char* p = str;*p != '\0';p++) {
		if (isalpha(*p)) {
			word += 1;
		}
		else if (isalnum(*p)) {
			num += 1;
		}
		else if (*p == ' ') {
			space += 1;
		}
		else {
			other += 1;
		}
	}
	printf("word:%d\nspace:%d\nnum:%d\nother:%d\n", word, space, num, other);
}
```
## 3.计算数学算式的值

求Sn = a + aa + aaa + aaaa + ... + aa...aaa的值，其中a表示一个数字，n表示a的位数，如果a等于2，那等于5，那么程序应该计算2 + 22 + 222 + 2222 + 22222的值。

```c
int GetSum(int a, int n) {
	int tmp = a, sum = a;
	for (int i = 1;i < n;i++) {
		tmp = tmp * 10 + a;
		sum = sum + tmp;
	}
	return sum;
}
```
## 4.求水仙花数

水仙花数是一个三位数，这个数等于其每一位数字开立方相加之和，求出所有水仙花数。

```c
void IsFlower() {//水仙花数
	int unit = 0, decade = 0, hundreds = 0;
	for (int i = 100;i < 1000;i++) {
		unit = i % 10;
		decade = i / 10 % 10;
		hundreds = i / 100;
		if (pow(unit, 3) + pow(decade, 3) + pow(hundreds, 3) == i) {
			printf("%d ", i);
		}
	}
}
```
## 5.求完数

完数是指这个数等于其所有因子之和（不包含这个数本身），求出1000以内所有的完数，并打印成以下格式：

> 6 its factors are 1,2,3

```c
bool IsPerfectNum(int n) {
	int tmp = 0;
	for (int i = 1; i < n;i++) {
		if (n % i == 0) {
			tmp += i;
		}
	}
	return tmp == n;
}
void PrintPerfectNum(int n) {
	printf("%d its factors are 1", n);
	for (int i = 2;i < n;i++) {
		if (n % i == 0) {
			printf(",%d", i);
		}
	}
}
//测试用例：
int main(){
	for (int i = 1;i < 1000;i++) {
		if (IsPerfectNum(i)) {
			PrintPerfectNum(i);
			printf("\n");
		}
	}
}
```
## 6.猴子吃桃

猴子第一天采了若干个桃子，吃了一半后又多吃了一个，然后每天都会吃吃前一天剩下桃子的一半又多吃一个，直到第十天桃子只剩下了一个。问第一天猴子采摘了多少个桃子？

```c
int Monky_Peach(int n) {//猴子吃桃
	int peach = 1;
	for (int day = n-1;day > 0;day--) {
		peach = (peach + 1) * 2;
	}
	return peach;
}

int Monky_Peach_recur(int n) {//猴子吃桃（递归）
	if (n == 1) {
		return 1;
	}
	else {
		return (Monky_Peach_recur(n - 1) + 1) * 2;
	}
}
```
## 7.比赛名单

两队分别派出A、B、C和X、Y、Z比赛，抽签决定对手，目前已知A不和X比，C不和X、Z比，那么请找出比赛的名单。

```c
void Print_List() {//比赛名单
	for (int a = 'X';a <= 'Z';a++) {
		for (int b = 'X';b <= 'Z';b++) {
			for (int c = 'X';c <= 'Z';c++) {
				if (a != 'X' && c != 'X' && c != 'Z' && a != b && a != c && b != c) {
					printf("A -- %c \nB -- %c \nC -- %c \n", a, b, c);
				}					
			}
		}
	}
}
```
## 8.编写calloc函数

```c
void* my_calloc(size_t num, size_t size) {
	void* ip = malloc(size * num);
	if (ip != nullptr) {
		memset(ip, 0, num * size);
	}
	return ip;
}
```
calloc函数会将堆区分配的内存初始化成零，首先调用malloc函数申请内存，然后采用memset函数将内存置为零。memset是<string.h>中的库函数，格式为void * memset ( void * ptr, int value, size_t num )，作用是将ptr指向的区域前num字节设置成value值。

## 9.输出魔方阵

所谓魔方阵是指这样的方阵，它的每一行、每一列和对角线之和均相等。要求输出1~n^2自然数奇数阶的魔方阵。

```c
void Magic_Square(int n) {
	assert(n % 2 != 0);//规定n只能为奇数
	int** arr = (int**)malloc(sizeof(int*) * n);//分配空间
	if (nullptr == arr)exit(1);
	for (int i = 0;i < n;i++) {
		arr[i] = (int*)calloc(n, sizeof(int));
		if (nullptr == arr[i])exit(1);
	}

	int priorRow = 0;
	int priorCol = n / 2;
	arr[priorRow][priorCol] = 1;//将数字1填入第一行中间

	for (int i = 2;i <= n * n;i++) {//依次将2~n*n填入矩阵
		if (arr[(priorRow - 1 + n) % n][(priorCol + 1) % n] == 0) {//当前位置的上一行下一列如果没有数字
			priorRow = (priorRow - 1 + n) % n;//更新当前位置坐标为之前位置的上一行下一列
			priorCol = (priorCol + 1) % n;
		}
		else {//如果有数字
			priorRow = (priorRow + 1) % n;//更新当前坐标为之前位置的下一行
		}
		arr[priorRow][priorCol] = i;//给矩阵中元素赋值
	}

	for (int i = 0; i < n; i++)	{//打印魔方阵
		for (int j = 0; j < n; j++) {
			printf("%4d", arr[i][j]);
		}
		printf("\n");
	}

	for (int i = 0;i < n;i++) {//释放空间
		free(arr[i]);
		arr[i] = nullptr;
	}
	free(arr);
	arr = nullptr;
}
```
魔方阵的构建规则如下：数字1只能填在第一行的中间；下一个数的位置应该在上一个数的上一行下一列；如果上一行下一列已经有了数字，那么下一个数应该在上一个数的下面。根据上面三条规则即可构建出魔方阵。

## 10.电文翻译

有一行电文，已按下面规律译成密码：

| 转换前（大写） | 转换后 |
| -------------- | ------ |
| A              | Z      |
| B              | Y      |
| C              | X      |
| ...            | ...    |

| 转换前（小写） | 转换后 |
| -------------- | ------ |
| a              | z      |
| b              | y      |
| c              | x      |
| ...            | ...    |


即第一个字母变成第26个字母，第i个字母变成第（26 - i + 1）个字母，非字母字符不变，要求编程序将密码译回原文，并输出密码和原文。

```c
void Code_To_Text(char* dest,const char* src, int n) {//电文转换
	assert(dest != nullptr && src != nullptr);
	int ar[26] = { 0 };

	for (int i = 1;i <= 26;i++) {//构建一个密码表
		ar[i - 1] = 26 - i;//ar[0] = 25 ar[1] = 24 ... ar[25] = 0
	}

	for (int i = 0;i < n;i++) {
		if (islower(src[i])) {//如果是小写字母
			char num = src[i] % 'a';
			dest[i] = ar[num] + 'a';
		}
		else if (isupper(src[i])) {//如果是大写字母
			char num = src[i] % 'A';
			dest[i] = ar[num] + 'A';
		}
		else {//如果是其他字符
			dest[i] = src[i];
		}
	}
	printf("Code: %s\n", src);
	printf("Text: %s\n", dest);
}
```
## 11.折半查找

有15个数按由大到小顺序放在一个数组中，输入一个数，要求用折半查找法找出该数是数组中第几个元素的值，如果该数不在数组中，则输出“无此数”。

```c
void BinarySearch(int* br, int n, int val) {//折半查找
	assert(br != nullptr);
	int pos = -1;
	int left = 0, right = n - 1;
	while (left <= right) {
		int mid = (right - left) / 2 + left;
		if (val > br[mid]) {
			right = mid - 1;
		}
		else if (val < br[mid]) {
			left = mid + 1;
		}
		else {
			pos = mid;
			break;
		}
	}
	if (pos == -1) {
		printf("无此数\n");
	}
	else {
		printf("%d\n", pos + 1);
	}
}
```
## 12.输出n行杨辉三角形

要求输出10行杨辉三角。

```c
void YangHui_Triangle(int n) {//杨辉三角形
	int** arr = (int**)malloc(sizeof(int*) * n);//分配空间
	if (nullptr == arr)exit(1);
	for (int i = 0;i < n;i++) {
		arr[i] = (int*)calloc(n, sizeof(int));
		if (nullptr == arr[i])exit(1);
	}

	for (int i = 0;i < n;i++) {//计算杨辉三角
		for (int j = 0;j <= i;j++) {
			if (j == 0 || i == j) {
				arr[i][j] = 1;
			}
			else {
				arr[i][j] = arr[i - 1][j - 1] + arr[i - 1][j];
			}
		}
	}
	
	for (int i = 0;i < n;i++) {//输出打印杨辉三角
		for (int j = 0;j <= i;j++) {
			printf("%4d", arr[i][j]);
		}
		printf("\n");
	}

	for (int i = 0;i < n;i++) {//释放内存
		free(arr[i]);
		arr[i] = nullptr;
	}
	free(arr);
	arr = nullptr;
}
```
## 13.用选择法对10个整数进行排序

```c
bool less(int a, int b) {//比较元素
	return a < b ? true : false;
}
void exchange(int* ar, int a, int b) {//交换元素
	int tmp = ar[a];
	ar[a] = ar[b];
	ar[b] = tmp;
}
void Select_Sort(int* ar,int n) {//选择查找
	assert(ar != nullptr);
	for (int i = 0;i < n;i++) {
		int min = i;
		for (int j = i + 1;j < n;j++) {
			if (less(ar[j], ar[min]))min = j;
		}
		exchange(ar, i, min);
	}
}
```
## 14.用筛选法筛选100以内的素数

```c
void Prime_Number(int n) {//筛选n以内的素数
	int* p = (int*)malloc(sizeof(int) * (n + 1));
	if (p == nullptr)exit(1);

	for (int i = 1;i <= n;i++) {
		p[i] = 1;
	}

	p[0] = p[1] = 0;//0和1不是素数

	for (int i = 1;i <= n;i++) {
		if (p[i] == 1) {//只有素数才能进入条件判断
			int j = i + 1;
			while (j <= n) {
				if (p[j] != 0 && j % i == 0) {//已筛除的数不用再判断能否被整除
					p[j] = 0;
				}
				j += 1;	
			}
		}
	}

	for (int i = 1;i <= n;i++) {//输出打印标记为1的所有数
		if (p[i] == 1) {
			printf("%4d ", i);
		}
	}

	free(p);
	p = nullptr;
}
```
## 15.输出元音字母

写一个函数，将一个字符串中的元音字母复制到另一字符串，然后输出。

```c
void Vowel(char* des, const char* src) {
	assert(des != NULL && src != NULL);
	const char* vowel = "aeiouAEIOU";
	int count = 0;
	while (*src != '\0') {
		for (int i = 0; i < 10; i++) {
			if (*src == vowel[i]) {
				des[count++] = *src;
				break;
			}
		}
		src++;
	}
	des[count] = '\0';
}
```
首先在程序中构建一个“元音字母表”，遍历字符串，对每一个字符进行查表，如果查找成功就将这个字符拷贝进另一个空字符串中，最后循环结束的时候不要忘了加上结束符。

## 16.输出最长单词

写一个函数，输入一行字符，将此字符串中最长的单词输出。

```c
char* MaxWords(const char* str)
{
	assert(str != nullptr);

	int len = strlen(str);
	char* curword = (char*)malloc(sizeof(char) * (len + 1));
	if (curword == nullptr)exit(1);
	char* maxword = (char*)malloc(sizeof(char) * (len + 1));
	if (maxword == nullptr)exit(1);
	int cur = 0;
	int max = 0;

	while (*str != '\0') {
		if (isalpha(*str)) {
			curword[cur++] = *str;
		}
		else {
			if (cur > max) {
				curword[cur] = '\0';
				strcpy_s(maxword, cur + 1, curword);
				max = cur;
				cur = 0;
			}
		}
		str++;
	}

	if (cur > max) {
		curword[cur] = '\0';
		strcpy_s(maxword, cur + 1, curword);
	}

	free(curword);
	curword = nullptr;

	return maxword;
}
```
构建两个字符串，curword负责记录当前字母，maxword记录最长字母，每次读取了一个完整字母时，将这个字母与当前最长单词比较，如果这个字母更长，则更新maxword中的内容。考虑特殊情况，如果用户给出的字符串就只有一个单词，那么直接将其拷贝至maxword中。

## 17.字符排列

写一个函数，用“起泡法”对输入的字符按由小到大顺序排序。

```c
void swap(char* a, char* b){
	char temp = *a;
	*a = *b;
	*b = temp;
}
void Bubblesort(char* ar) {
	assert(ar != nullptr);
	int len = strlen(ar);
	for (int i = 0;i < len - 1;i++) {
		bool flag = true;
		for (int j = 0;j < len - 1 - i;j++) {
			if (ar[j] > ar[j + 1]) {
				swap(&ar[j], &ar[j + 1]);
				flag = false;
			}
		}
		if (flag)break;
	}
}
```
## 18.数据移动

有n个整数，使前面各数顺序向后移动m个位置，最后m个数变成最前面m个数，实现这个功能。

```c
void Move_Ar(int* ar, int n) {
	assert(ar != nullptr);
	int tmp = ar[n - 1];
	for (int i = n - 1;i > 0;i--) {
		ar[i] = ar[i - 1];
	}
	ar[0] = tmp;
}
void Move_Ar_m(int* ar, int n, int m) {
	assert(ar != nullptr);
	m = m % n;
	for (int i = 0;i < m;i++) {
		Move_Ar(ar, n);
	}
}
```
编写Move_Ar函数，这是移动数组一个元素的函数，然后在Move_Ar_m中调用它，就可以完成移动数组m次的功能。

## 19.内存移动函数

```c
void my_memmove(void* dest, const void* src, size_t count) {
	char* dst = (char*)dest;
	const char* sr = (const char*)src;

	dst = dst + count - 1;
	sr = sr + count - 1;

	while (count--) {
		*dst = *sr;
		dst--;
		sr--;
	}
}
//测试用例：
int main() {
	char str[] = { "memmove can be very useful......" };
	my_memmove(str + 20, str + 15, 11);
	printf("%s", str);
	return 0;
}
```
最终输出结果："memmove can be very very useful."。注意函数的参数，前两个是无类型指针，使用了C语言泛型编程的特性：无类型指针可以接收任何类型的指针，但是要想使用它要将其进行强制类型转换。第三个参数是移动的字节数，对于char类型来说，移动多少个字符就是移动多少个字节，但是对于其他数据类型来说比如整型就不是这样，因为整型占4个字节，比如用户想要移动n个整型数，需要移动的字节数是4n，这一点需要注意。

## 20.报数问题

有n个人围成一圈，顺序排号。从第1个人开始报数（从1到3报数），凡报到3的人退出圈子，问最后留下的原来第几号的那位？

```c
int JosephusProblem(int n) {
	assert(n > 1);
	int* arr = (int*)malloc(sizeof(int) * n);
	if (arr == nullptr)exit(1);
	for (int i = 0;i < n;i++) {
		arr[i] = 1;
	}
	int sum = n;
	int count = 0;
	while (sum > 1) {
		for (int i = 0;i < n;i++) {
			if (arr[i] == 1) {
				count += 1;
				if (count == 3) {
					count = 0;
					arr[i] = 0;
					sum -= 1;
				}
			}
		}
	}
	for (int i = 0;i < n;i++) {
		if (arr[i] == 1) {
			free(arr);
			arr = nullptr;
			return i;
		}
	}
	free(arr);
	arr = nullptr;
	return -1;
}
```
开辟n个长度数组代表n个人，一开始设将数组中的每一个值置为1。遍历整个数组，当计数器记到3的时候，将数组下标对应的元素置为0，只有当元素值为1时计数器才会计数。与此同时人数减一，计数器重新从0到3计数，如此循环往复知道人数减少到1的时候，退出循环，此时数组中应该只有一个下标对应的元素值为1，其余均为0。所以我们再一次遍历数组，找到值为1的元素下标，这就是留下来的人的位置。

## 21.字符串截取

有一字符串，包含n个字符。写一个函数，将此字符串中从第m个字符开始的全部字符复制成为另一个字符串。

```c
void StrCut(char* dest, const char* src, int m) {
	assert(dest != nullptr && src != nullptr);
	int n = strlen(src);
	if (m < 1 || m > n) {
		*dest = '\0';
		return;
	}

	for (int i = 1;i < m;i++) {
		src++;
	}
	while (*src != '\0') {
		*dest = *src;
		src++;
		dest++;
	}
	*dest = '\0';
}
```
题目要求从字符串第m个字符开始复制后面全部字符，所以设置一个指针将其移动到第m个字符上，然后依次遍历每一个字符直到字符串末尾即可。注意条件判断，用户给出的m值不能小于1或者大于原来字符串的最大长度。

## 22.字符串排序

在主函数中输入10个等长的字符串，用另一个函数对它们排序，然后在主函数输出这10个已排好序的字符串。

```c
void StrSort(const char** str, int len) {
	assert(str != nullptr);
	for (int i = 0;i < len - 1;i++) {
		int min = i;
		for (int j = i + 1;j < len;j++) {
			if (strcmp(str[min], str[j]) > 0) {
				min = j;
			}
		}
		if (min != i) {
			const char* tmp = str[min];
			str[min] = str[i];
			str[i] = tmp;
		}
	}
}
```
通过选择排序来对每一个字符串进行字典序排序。注意参数需要传递二级指针或者一个指针数组。

## 23.从字符串中取出整数

输入一个字符串，内有数字和非数字字符，例如：

> A123x456 17960? 302tab5876

将其中连续的数字作为一个整数，依次存放到一数组a中。例如，123放在a[0]，456放在a[1]......统计共有多少个整数，并输出这些数。

```c
int GetNumCount(char* str) {
	assert(str != nullptr);
	int count = 0;
	while (*str != '\0') {
		if (isdigit(*str) && !isdigit(*(str + 1))) {
			count += 1;
		}
		str++;
	}
	return count;
}
int* GetNumArrFormStr(char* str) {
	assert(str != nullptr);
	int num = GetNumCount(str);
	int* iar = (int*)malloc(sizeof(int) * num);
	if (iar == nullptr)exit(1);

	bool tag = false;
	int sum = 0;
	int i = 0;
	while (*str != '\0') {
		while(isdigit(*str)) {//是数字
			tag = true;
			sum = sum * 10 + *str - '0';
			str++;
		}
		if (tag != false) {
			iar[i] = sum;
			i++;
			sum = 0;
			tag = false;
		}
		else {//不是数字
			str++;
		}
	}
	return iar;
}
```
首先统计字符串中有多少整数，然后对应开辟数组。设置一个标记，当检测到数字时为真，只有当标记是真的时候将这个整数存入数组中，最后程序将这个数组的首地址返回，得到最终结果。

## 24.求某一天是一年中的第几天

定义一个结构体变量（包括年、月、日），计算该日在本年中是第几天，考虑闰年的问题。

```c
typedef struct {
	int year;
	int month;
	int day;
}date;
bool isleapyear(int year) {
	return (year % 4 == 0 && year % 100 != 0 || year % 400 == 0);
}
int monthdays(int year,int month) {
	static const int days[13] = { 29,31,28,31,30,31,30,31,31,30,31,30,31 };
	if (month == 2 && isleapyear(year)) {
		//二月份且是闰年
		month = 0;
	}
	return days[month];
}
int totaldays(date* ymd) {
	assert(ymd != nullptr);
	int total = 0;
	if (ymd->year < 1)return -1;//年非法
	if (ymd->month < 1 || ymd->month > 12)return -2;//月非法
	if (ymd->day < 1 || ymd->day > monthdays(ymd->year, ymd->month))return -3;//天非法

	for (int i = 1;i < ymd->month;i++) {
		total = total + monthdays(ymd->year, i);
	}
	total = total + ymd->day;
	return total;
}
```
注意闰年的判断方法：可以被4整除同时不能被100整除；或者可以被400整除。采用数组保存一年中每个月的天数，并且在主函数中判断年、月、日非法的情况。

## 25.输出学生的信息

有10个学生，每个学生的数据包括学号、姓名、3门课的成绩。从键盘输入10个学生数据，并用输出函数打印到屏幕上。同时要求输出这三门课的平均分，以及最高分学生的数据（包括学号、姓名、4门课的成绩、平均分数）。

```c
typedef struct {
	char num[20];
	char name[20];
	double score[3];
}student;
void printstudent(student* stu, int len) {
	assert(stu != nullptr);
	for (int i = 0;i < len;i++) {
		printf("num : %s\tname : %s\tscore1 : %f\tscore2 : %f\tscore3 : %f\n",
			stu[i].num, stu[i].name, stu[i].score[0], stu[i].score[1], stu[i].score[2]);
	}
	printf("\n");
}
void inputstudent(student* stu, int len) {
	assert(stu != nullptr);
	for (int i = 0;i < len;i++) {
		printf("请输入第%d个学生的信息：\n", i + 1);
		scanf_s("%s%s%lf%lf%lf", &stu[i].num, 20, &stu[i].name, 20, &stu[i].score[0], &stu[i].score[1], &stu[i].score[2]);
	}
}
void printscore(student* stu, int len) {
	assert(stu != nullptr);	
	double average1 = 0;
	double average2 = 0;
	double average3 = 0;
	for (int i = 0;i < len;i++) {
		average1 += stu[i].score[0];
		average2 += stu[i].score[1];
		average3 += stu[i].score[2];
	}
	average1 = average1 / 3;
	average2 = average2 / 3;
	average3 = average3 / 3;
	printf("average\tscore1\tscore2\tscore3\n\t%.2f\t%.2f\t%.2f\n", average1, average2, average3);
	double stuaverage = 0;
	int n = 0;
	for (int i = 0;i < len;i++) {
		double tmp = (stu[i].score[0] + stu[i].score[1] + stu[i].score[2]) / 3;
		if (tmp > stuaverage) {
			stuaverage = tmp;
			n = i;
		}
	} 
	printf("num\t%s\tname\t%s\tscore1\t%.f\tscore2\t%.f\tscore3\t%.f\taverage\t%.2f\n",
		stu[n].num, stu[n].name, stu[n].score[0], stu[n].score[1], stu[n].score[2], stuaverage);
}
```
关于最高分学生的判断，计算每一名学生的平均分数，逐个进行比较，记录下最高的平均分和对应的学生，然后输出即可。

## 26.输入输出控制

从键盘输入一个字符串，将其中的小写字母转换成大写字母，然后输出到一个磁盘文件中保存，输入的字符以感叹号结尾。

```c
void ShowToUpper()
{
	const char* path = "D:\\VS2019程序代码\\Test7_21\\test.txt";
	FILE * fw = nullptr;
	errno_t err = fopen_s(&fw, path, "w");
	assert(fw != nullptr);

	char ch;
	while ((ch = getchar()) != '!')
	{
		if (islower(ch))
		{
			ch = toupper(ch);
		}
		fwrite(&ch, sizeof(char), 1, fw);
	}
	fclose(fw);
	fw = nullptr;
}
```
其中将小写字母转换成大写字母的过程使用了ctype.h头文件中的库函数，一个是判断是否是小写字母islower，另一个是将小写字母转换为大写字母toupper。

## 27.文件合并

将两个磁盘文件中各存放一行字母，现在要求将这两个文件中的信息合并到一个新的文件中，并按照字母顺序排序。

```c
void Merge(const char* pathA, const char* pathB, const char* pathC)
{
	assert(pathA != nullptr && pathB != nullptr && pathC != nullptr);
	FILE* fw = nullptr;
	FILE* frA = nullptr;
	FILE* frB = nullptr;

	errno_t errc = fopen_s(&fw, pathC, "w");
	errno_t erra = fopen_s(&frA, pathA, "r");
	errno_t errb = fopen_s(&frB, pathB, "r");
	assert(fw != nullptr && frA != nullptr && frB != nullptr);
	
	char ch_a = '\0';
	char ch_b = '\0';
	int len1 = 0;//通过fread从文件A中读取的完整的项目个数
	int len2 = 0;//通过fread从文件B中读取的完整的项目个数

	len1 = fread(&ch_a, sizeof(char), 1, frA);
	len2 = fread(&ch_b, sizeof(char), 1, frB);

	while (len1 > 0 && len2 > 0)//两个文件都还有数据
	{
		if (ch_a <= ch_b)
		{
			fwrite(&ch_a, sizeof(char), 1, fw);
			len1 = fread(&ch_a, sizeof(char), 1, frA);
		}
		else
		{
			fwrite(&ch_b, sizeof(char), 1, fw);
			len2 = fread(&ch_b, sizeof(char), 1, frB);
		}
	}

	while (len1 > 0)//A文件没读取完，B读取完了
	{
		fwrite(&ch_a, sizeof(char), 1, fw);
		len1 = fread(&ch_a, sizeof(char), 1, frA);
	}

	while (len2 > 0)//B文件没读取完，A读取完了
	{
		fwrite(&ch_b, sizeof(char), 1, fw);
		len2 = fread(&ch_b, sizeof(char), 1, frB);
	}

	fclose(fw);
	fw = nullptr;
	fclose(frA);
	frA = nullptr;
	fclose(frB);
	frB = nullptr;
}
```
首先创建好三个磁盘文件，其中两个为只读方式打开，作为待归并文件，而第三个以只写形式打开，作为目标文件。每次从两个待归并文件中读取出一个字母，然后比较，将小的字母写入目标文件中，然后继续读取下一个字母进行比较，以此类推。直到其中任意一个文件读完，然后将另一个没有读完的文件中剩下的内容写入到新文件中。

# 动态内存管理题目

## 1.打家劫舍

假如你是一名专业的小偷，可以进入住户家中进行偷窃。现在有一个街区若干家住户，每个住户家中都有一定数量的现金，限制你偷窃的唯一问题是相互连通的报警系统，如果你连续偷了相邻两家住户的现金，那么报警器将会报警。那么对于这样一个街区，你应该如何规划偷窃的住户，使自己获得最大的收益？

```c
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

int Max_Int(int a, int b) {
	return a > b ? a : b;
}
int Rob(int* nums, int n) {//打家劫舍
	assert(nums != nullptr);
	int* p = (int*)malloc(sizeof(int) * n);
	if (p == nullptr)exit(-1);
	for (int i = 0;i < n;i++) {
		p[i] = 0;
	}
	if (n == 1)return nums[0];
	if (n == 2)return nums[0] > nums[1] ? nums[0] : nums[1];
	p[0] = nums[0];
	p[1] = nums[0] > nums[1] ? nums[0] : nums[1];
	for (int i = 2;i < n;i++) {
		p[i] = Max_Int(p[i - 1], p[i - 2] + nums[i]);
	}
	int max = p[n - 1];
	free(p);
	p = nullptr;
	return max;
}

int main() {
	int nums[] = { 2,7,9,3,1 };
	int n = sizeof(nums) / sizeof(nums[0]);
	printf("%d", Rob(nums, n));
	return 0;
}
```

程序的关键是设置一个数组p，p[i]表示偷第i家所能获得的最大利润。动态规划的方程式如下：

> p[i] = Max(p[i - 1], p[i - 2] + nums[i])

简单来说，对于第i个住户，我们比较不偷这一家或者偷这一家所产生的收益，将大的一方赋值给p[i]，然后以此类推，直到推算出偷最后一家住户所能产生的最大收益为止。如果有n家住户，那么p[n-1]就是程序最终的结果，我们将其作为返回值返回即可。

## 2.机器人路径

有一个m\*n的棋盘，一个机器人在左上角，一次可以挪动一格，只可以进行向下或者右移动，终点在棋盘的右下角。请计算出在这个m\*n的棋盘里，机器人有多少种到达右下角终点的路径？

```c
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>


int Robot(int m,int n) {//机器人路径
	if (m == 1 || n == 1)return 1;

	int** arr = (int**)malloc(sizeof(int*) * m);
	if (arr == nullptr)exit(-1);
	for (int i = 0;i < m;i++) {
		arr[i] = (int*)malloc(sizeof(int) * n);
		if (arr[i] == nullptr)exit(-1);
	}

	for (int r = 0;r < m;r++) {
		arr[r][0] = 1;
	}
	for (int c = 0;c < n;c++) {
		arr[0][c] = 1;
	}
	int i = 0, j = 0;
	for (i = 1;i < m;i++) {
		for (j = 1;j < n;j++) {
			arr[i][j] = arr[i - 1][j] + arr[i][j - 1];
		}
	}
	int num = arr[i - 1][j - 1];
	for (int i = 0;i < m;i++) {
		free(arr[i]);
		arr[i] = nullptr;
	}
	free(arr);
	arr = nullptr;
	return num;
}

int main() {
	printf("%d", Robot(7, 3));
	return 0;
}
```

程序的关键是如何动态开辟一个二维数组空间。如果想得到m行n列的一个数组，那么首先应该开辟一个有m个元素指针数组，并由二级指针指向其首元素地址；然后对这个指针数组中每一个元素开辟n个整形空间，这样就构建出了一个二维数组。得到二维数组之后，我们通过二重循环遍历数组中的元素，计算到任意一个单元格的走法。由于机器人只能向右或者向下移动，所以我们要计算某一个单元格的走法，就需要计算其左边和上边单元格的走法，累加就可以得到这个单元格的走法。方程式如下：

> arr[i]\[j] = arr[i - 1]\[j] + arr[i]\[j - 1];

并且可以看出，如果行数和列数有任意一个等于一，那么机器人就只有一种走法走到终点。结合以上特点，就可以通过我们动态开辟的二维数组得到机器人到达右下角有多少种走法。
