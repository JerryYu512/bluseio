/**
 * MIT License
 * 
 * Copyright © 2022 <Jerry.Yu>.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS";, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * @file singleton.h
 * @brief 
 * @author  Jerry.Yu(astralrovers@outlook.com)
 * @version 1.0.0
 * @date 2022-12-14
 * 
 * @copyright MIT License
 * 
 */
#pragma once

namespace bluseio
{

namespace base
{

template <class T>
class SingleTon
{
public:
    static T &GetInstance()
    {
        static T ins;
        return ins;
    }
    SingleTon(const SingleTon &) = delete;
    SingleTon &operator=(const SingleTon &) = delete;
    virtual ~SingleTon() {}

protected:
    SingleTon() {}
};

/**
 * @usage:
class Apple : public SingleTon<Apple>
{
    friend SingleTon<Apple>;

public:
    void show()
    {
        cout << __FUNCTION__ << endl;
    }
    ~Apple()
    {
        cout << __FUNCTION__ << endl;
    }

protected:
    Apple() {}
};

Apple &apple = SingleTon<Apple>::GetInstance();

*/

}

}