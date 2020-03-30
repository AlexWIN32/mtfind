class Strategy
{
public:
    template<typename TVal>
    static TVal A()
    {
        return TVal();
    }
    static int B()
    {
        return 0;
    }
};

template <typename TStrategy>
class User
{
public:
    template<typename TVal>
    TVal A()
    {
        return TStrategy::A<TVal>();
	//return TStrategy::B();
    }
};

int main()
{
    User<Strategy> user;
}
