package lab;


/**
* lab/GroundHelper.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from root.idl
* wtorek, 24 kwiecie� 2012 02:57:52 CEST
*/

abstract public class GroundHelper
{
  private static String  _id = "IDL:lab/Ground:1.0";

  public static void insert (org.omg.CORBA.Any a, lab.Ground that)
  {
    org.omg.CORBA.portable.OutputStream out = a.create_output_stream ();
    a.type (type ());
    write (out, that);
    a.read_value (out.create_input_stream (), type ());
  }

  public static lab.Ground extract (org.omg.CORBA.Any a)
  {
    return read (a.create_input_stream ());
  }

  private static org.omg.CORBA.TypeCode __typeCode = null;
  synchronized public static org.omg.CORBA.TypeCode type ()
  {
    if (__typeCode == null)
    {
      __typeCode = org.omg.CORBA.ORB.init ().create_interface_tc (lab.GroundHelper.id (), "Ground");
    }
    return __typeCode;
  }

  public static String id ()
  {
    return _id;
  }

  public static lab.Ground read (org.omg.CORBA.portable.InputStream istream)
  {
    return narrow (istream.read_Object (_GroundStub.class));
  }

  public static void write (org.omg.CORBA.portable.OutputStream ostream, lab.Ground value)
  {
    ostream.write_Object ((org.omg.CORBA.Object) value);
  }

  public static lab.Ground narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof lab.Ground)
      return (lab.Ground)obj;
    else if (!obj._is_a (id ()))
      throw new org.omg.CORBA.BAD_PARAM ();
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      lab._GroundStub stub = new lab._GroundStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

  public static lab.Ground unchecked_narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof lab.Ground)
      return (lab.Ground)obj;
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      lab._GroundStub stub = new lab._GroundStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

}
