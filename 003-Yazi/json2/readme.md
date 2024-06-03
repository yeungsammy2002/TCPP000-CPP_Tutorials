# Implicitly using Copy Constructor, Copy Assignment Operator, Move Assignment Operator


## Deep Copy for user-define object that contain pointer member(s) is a must
For any user-defined object that contain(s) **pointer(s)**, or a **collection(s) of pointers**, those ***copy constructor*** and ***copy assignment operator*** **MUST BE implemented using DEEP COPY**. Especially those member is a collections of pointers, i.e. a **`std::vector`** of pointers.

Let use our Json object as an example. When the existing Json object that put into a function as an argument, the existing object will hold the memory address that points to the area that has already "deleted" after the function called.

As the last statement of main() executed, existing Json object's destructor will be called and the deleted memory will be deleted again, in other words, you delete the same memory area twice, which will caused the error.

The solution to this is the members of two different **`JSON_ARRAY`** or **`JSON_OBJECT`** type Json objects **MUST NOT** point to the same memory.


## Standard Libraray do not clearly specify which kind of user-defined object's constructors or operator overloading methods are being called when calling their container's methods


### `std::vector<T>`'s `push_back()` calling User-Define Object's Copy Constructor & Destructor
For example, we don't know **`std::vector`**'s **`push_back()`** method is actually called ***copy constructor*** to copy all members of the existing structure to a new vector structure when the ***capacity*** of existing vector is not enough. After the copying process finished, **all the members of the original vector will be destroyed by calling their *destructors***.

```
m_value.m_array = new std::vector<Json>();
auto o_begin = other.m_value.m_array->begin();
auto o_end = other.m_value.m_array->end();
for (auto o_it = o_begin; o_it != o_end; ++o_it)
{
    m_value.m_array->push_back(*o_it);              // copy constructor(s) called first, then destructor(s)
}                                                   // if capacity of existing vector is not enough
```
Once ***destructor*** delete the memory area that the pointer points to, we don't know the vector will also be affected because we don't know the vector structure is already updated, all the members are lived in another memory area. Now all the members are own the memory that are already deleted.


### `std::vector<T>`'s `erase()` calling User-Define Object's Move Assignment Operator or Copy Assignment Operator
Another example is we don't know **`std::vector<T>::erase(iterator)`** is actually called their ***move assignment operator***. If their ***move assignment operator overloading*** is not implemented, **`std::vector<T>::erase(iterator)`** will use their ***copy assignment operator***.

For example, when a std::vector object erase its first element, copy assignment operator will be called in order to move its member to appropriate position. The second element will be copied at first, and then paste to the first element position of a newly created std::vector structure.

The problem is after the copy and paste process is finished. The original std::vector structure and all members inside the structure will be destroyed by calling its destructor. We only keep the updated version of the previous std::vector structure, but the members of the updated structure are actually own the memory that already deleted. After we call **`std::vector<T>::erase(iterator)`**, the error occurred.

Let's say we have **`JSON_ARRAY`** type Json object called **`arr`** that contains 3 **`JSON_ARRAY`** type Json object:
```
arr: [str_array1, str_array2, str_array3]
```

`arr` now wants to remove the first element **`str_array1`**:
```
arr.m_value.m_array->erase(arr.begin());
```

Under the hood, what `std::vector` will do is to create a new memory area, then move **`str_array2`** to the **first position** of the new area, and then move **`str_array3`** to the **second position** of the new area:
```
[str_array2, str_array3]
```
When the moving start, ***move assignment operator overloading method*** will be called. If ***move assignment operator overloading*** doesn't exist, ***copy assignment operator*** will be called. Once ***copy assignment operator*** called, its ***destructor*** will also be called inevitably.


### `std::map<T_first, T_second>`'s subscript operator calling User-Define Object's Move Constructor, Copy Assignment Operator & Destructor
```
Json o(Json::JSON_OBJECT);
o["name"] = "jack";
```

Here are the steps which constructors, operators, or methods are being called:
1. Json ***constructor takes c string*** **`"jack"`** will be called to create temporary object
2.  ***Subscript operator overloading method takes c string*** **`"name"`** will be called and pass the temporary Json object into the method
3. ***Move assignment operator overloading method takes Json object*** will be called, but ***copy assignment operator overloading*** are being called if ***move assignment operator*** not implemented. Once ***copy assignment operator*** called, its ***destructor*** will also be called inevitably.

