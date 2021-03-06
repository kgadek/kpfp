// **********************************************************************
//
// Copyright (c) 2003-2011 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.4.2
//
// <auto-generated>
//
// Generated from file `Lambda.java'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

package blackmesa.testlab.devices;

public class Lambda implements java.lang.Cloneable, java.io.Serializable
{
    public String name;

    public ElementType _return;

    public ElementType[] parameters;

    public Lambda()
    {
    }

    public Lambda(String name, ElementType _return, ElementType[] parameters)
    {
        this.name = name;
        this._return = _return;
        this.parameters = parameters;
    }

    public boolean
    equals(java.lang.Object rhs)
    {
        if(this == rhs)
        {
            return true;
        }
        Lambda _r = null;
        try
        {
            _r = (Lambda)rhs;
        }
        catch(ClassCastException ex)
        {
        }

        if(_r != null)
        {
            if(name != _r.name)
            {
                if(name == null || _r.name == null || !name.equals(_r.name))
                {
                    return false;
                }
            }
            if(_return != _r._return)
            {
                if(_return == null || _r._return == null || !_return.equals(_r._return))
                {
                    return false;
                }
            }
            if(!java.util.Arrays.equals(parameters, _r.parameters))
            {
                return false;
            }

            return true;
        }

        return false;
    }

    public int
    hashCode()
    {
        int __h = 0;
        if(name != null)
        {
            __h = 5 * __h + name.hashCode();
        }
        if(_return != null)
        {
            __h = 5 * __h + _return.hashCode();
        }
        if(parameters != null)
        {
            for(int __i0 = 0; __i0 < parameters.length; __i0++)
            {
                if(parameters[__i0] != null)
                {
                    __h = 5 * __h + parameters[__i0].hashCode();
                }
            }
        }
        return __h;
    }

    public java.lang.Object
    clone()
    {
        java.lang.Object o = null;
        try
        {
            o = super.clone();
        }
        catch(CloneNotSupportedException ex)
        {
            assert false; // impossible
        }
        return o;
    }

    public void
    __write(IceInternal.BasicStream __os)
    {
        __os.writeString(name);
        _return.__write(__os);
        TypeListHelper.write(__os, parameters);
    }

    public void
    __read(IceInternal.BasicStream __is)
    {
        name = __is.readString();
        _return = ElementType.__read(__is);
        parameters = TypeListHelper.read(__is);
    }
}
