#include "Playground.h"

#include <functional>
#include <string>

#define UNUSED(x) (void)(x)


Playground::Playground()
{
}


Playground::~Playground()
{
}


int getConstFive() { return 5; }
constexpr int getConstExprFive() { return 5; }

void Playground::Run()
{
    SyntaxUpgrades();
}

template<typename T>
class SomeInitializedClass
{
    int myInt = 1;
    float myFloat = 2.0f;
    const char* myStr = "hello";
    std::string myString = "world";
    std::vector<int> myVec = { 1, 2, 3 };
    T t = 5;
};

void Playground::SyntaxUpgrades()
{
    SomeInitializedClass<int> myshit;

    //////////////////////////////////////////////////////////////////////////
    // Feature: constant expressions (constexpr):
    //////////////////////////////////////////////////////////////////////////

    //int arr[getConstFive()+2];    // This is invalid. Normal functions run at runtime. X needs to be constant in arr[x]
    int arr[getConstExprFive() + 2]; // This is valid. function evaluated at compile time.

    //////////////////////////////////////////////////////////////////////////
    // Feature: initializers:
    //////////////////////////////////////////////////////////////////////////
    struct ABStruct {
        float A;
        int B;
    };
    struct ABSequence {
        ABSequence(std::initializer_list<ABStruct> seq) :mySeq(seq) { }
        std::vector<ABStruct> mySeq;
    };
    // you can (still) initialize structs simply.
    ABStruct abObj = { 0.0f, 0 };
    // you can now initialize arrays this way. 
    ABStruct abArr[] = { { 0.1f, 1 },{ 0.2f, 2 },{ 0.3f, 3 } };
    // you can also initialize base types with this syntax.
    std::vector<ABStruct> abVec = { { 0.4f, 4 },{ 0.5f, 5 },{ 0.5f, 5 } };
    // using std::initializer_list<T> you can create constructors that work this way.
    ABSequence abSeq = { { 0.6f, 6 },{ 0.7f, 7 },{ 0.8f, 8 } };

    //////////////////////////////////////////////////////////////////////////
    // Feature: auto:
    //////////////////////////////////////////////////////////////////////////
    for (std::vector<ABStruct>::iterator it = abVec.begin(); it != abVec.end(); it++) { /* printf("ab(%f,%d\n)", it->A, it->B); */ }

    // you can substitute complex types with the term "auto".
    for (auto it = abVec.begin(); it != abVec.end(); it++) { /* printf("ab(%f,%d\n)", it->A, it->B); */ }

    // you can also use it to avoid being redundant
    auto myStr = ABStruct({1.0f, 1});
    // instead of:
    // ABStruct myStr = ABStruct({ 1.0f, 1 });

    //////////////////////////////////////////////////////////////////////////
    // Feature: range based for loop:
    //////////////////////////////////////////////////////////////////////////
    for (auto& ab : abVec) {
        //printf("ab(%f,%d)\n", ab.A, ab.B);
    }

    //////////////////////////////////////////////////////////////////////////
    // Feature: lambdas:
    //////////////////////////////////////////////////////////////////////////
    auto add = [](int x, int y) { return x + y; }; // you can make very simple lambdas
    auto mult = [](int x, int y) -> int { return x * y; }; // you can be explicit with return types
    int ten = add(5, 5);

    //auto addToAbObj = [](float x, int y) { abObj.A += x; abObj.B += y; }; // This doesn't work, we don't have abObj in scope.
    auto addToAbObj = [abObj](float x, int y) mutable { abObj.A += x; abObj.B += y; }; // this works, we captured abObj into the function scope. mutable is required
    //auto addToAbObj = [&](float x, int y) { abObj.A += x; abObj.B += y; }; // this works too, it captures everything in scope.
    addToAbObj(10.0f, 10);

    // we can use std::function to pass lambdas around (to functions, for example) and store them in classes.
    std::function<int(int, int)> someMathOp = add;
    ten = someMathOp(5, 5); // up, still 10.

    //////////////////////////////////////////////////////////////////////////
    // just to make sure the compiler doesn't whine about unused vars.
    //////////////////////////////////////////////////////////////////////////
    UNUSED(arr);
    UNUSED(abObj);
    UNUSED(abArr);
    UNUSED(abVec);
    UNUSED(abSeq);
    UNUSED(add);
}