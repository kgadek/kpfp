package lab;

/**
* lab/MutaliskHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from root.idl
* wtorek, 24 kwiecień 2012 14:51:11 CEST
*/

public final class MutaliskHolder implements org.omg.CORBA.portable.Streamable
{
  public lab.Mutalisk value = null;

  public MutaliskHolder ()
  {
  }

  public MutaliskHolder (lab.Mutalisk initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = lab.MutaliskHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    lab.MutaliskHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return lab.MutaliskHelper.type ();
  }

}
