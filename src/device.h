#ifndef SUPA_DEVICE_BASE_H
#define SUPA_DEVICE_BASE_H

class Device
{  /// Basic device class that provides root for all input and output devices.
   
public:
   /// Initialize the device, the @param name must point to valid memory at all times.
   Device( const char* name );
   
   /// Return json string like '"led" : { "value" : 1}}'. @returns the length of the added content.
   virtual int jsonify( char* buffer, int buffer_len ) ; 
   virtual void loop();
   virtual void setup();
   
protected:
   const char* name;
   int value; /// Implementing class must have loop() providing updates to this value.  
};


class Device_input: public Device
{
   
public:
   Device_input( const char* name );
  
   /// @returns sensor current reading
   int get_value() const;
protected:
   // update value with extra print
   void update_value(int new_value);
};


class Device_output: public Device
{
   
public:
   Device_output( const char* name );
   /// @param value new value, @param verbose if set to true then will have log print if value changed
   virtual void set_value( int value, bool verbose=false ) = 0;
  
};

#endif
