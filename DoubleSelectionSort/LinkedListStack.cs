using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DoubleSelectionSort;

public class LinkedListStack<T> : LinkedList<T>
{
    public LinkedListStack(int count = 0)
    {

    }

    public LinkedListStack<T> Push(T item)
    {
        AddLast(item);
        return this;
    }
    public T Pop()
    {
        var top = this.Last.Value;
        this.RemoveLast();
        return top;
    }
    public T Peek() => this.Last.Value;
}
