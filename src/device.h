#ifndef SUPA_DEVICE_BASE_H
#define SUPA_DEVICE_BASE_H

class Device
{  /// Basic device class that provides root for all input and output devices.
   
public:
   /// Initialize the device, the @param name must point to valid memory at all times.
   Device( const char* name );
   
   /// Return json string like '"led" : { "value" : 1}}'. @returns the length of the added content.
   virtual int jsonify( char* buffer, int buffer_len ) = 0;
   virtual void loop();
   
protected:
     const char* name;
};


class Device_input: public Device
{
   
public:
   Device_input( const char* name );
   virtual int jsonify( char* buffer, int buffer_len ) override ; 
   /// @returns sensor current reading
   int get_value() ;
   
protected:
   int value; /// Implementing class must have loop() providing updates to this value.  
};


class Device_output: public Device
{
   
public:
   Device_output( const char* name );
   virtual int jsonify( char* buffer, int buffer_len ) override; 
   virtual void set_value( int value ) = 0;
   
protected:
   int value; /// Implementing class must handle the writing of this value. 
   
};

#endif
