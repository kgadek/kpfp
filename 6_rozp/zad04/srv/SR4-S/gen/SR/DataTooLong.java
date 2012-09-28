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
// Generated from file `DataTooLong.java'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

package SR;

public class DataTooLong extends Ice.UserException
{
    public String
    ice_name()
    {
        return "SR::DataTooLong";
    }

    public void
    __write(IceInternal.BasicStream __os)
    {
        __os.writeString("::SR::DataTooLong");
        __os.startWriteSlice();
        __os.endWriteSlice();
    }

    public void
    __read(IceInternal.BasicStream __is, boolean __rid)
    {
        if(__rid)
        {
            __is.readString();
        }
        __is.startReadSlice();
        __is.endReadSlice();
    }

    public void
    __write(Ice.OutputStream __outS)
    {
        Ice.MarshalException ex = new Ice.MarshalException();
        ex.reason = "exception SR::DataTooLong was not generated with stream support";
        throw ex;
    }

    public void
    __read(Ice.InputStream __inS, boolean __rid)
    {
        Ice.MarshalException ex = new Ice.MarshalException();
        ex.reason = "exception SR::DataTooLong was not generated with stream support";
        throw ex;
    }
}