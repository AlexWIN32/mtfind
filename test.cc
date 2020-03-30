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

/*
template<typename TVal>
TVal Strategy::A()
{
    return TVal();
}

int Strategy::B()
{
    return 0;
}
*/

template<typename TStrategy>
class User
{
public:
    template<typename TVal>
    TVal A()
    {
	return TStrategy::template A<TVal>();
        //TStrategy obj;
	//return obj.A<TVal>();
    }
};

/*
template<typename TStrategy>
template<typename TVal>
TVal User<TStrategy>::A()
{
    TStrategy tp;
    return tp.A<int>();
}
*/

int main()
{
    User<Strategy> user;
    user.A<int>();
}
