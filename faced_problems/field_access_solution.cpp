class B;

class A {
protected:
    int x;
private:
    int y;
};

class A_wrapper : public A {
    friend B;
};


class B {
public:
    A_wrapper a;
    int foo() {
        a.x;   // Ok
        a.y;   // Compiler error!
    }
};
