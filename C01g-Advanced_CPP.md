# Section 23 - Duality of Public Inheritance - Interface & Implementation

```
class Dog {
public:
    virtual void bark() = 0;
};

class YellowDog : public Dog {
public:
    virtual void barks() {
        std::cout << "I am a yellow dog.\n";
    }
}
```