#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 9)
#ifndef SDL_sensor_hpp_
#define SDL_sensor_hpp_
#pragma once

#include <SDL_sensor.h>

#include "container.hpp"

#include <memory>

namespace SDL
{
	/**
	 * This is a unique ID for a sensor for the time it is connected to the system,
	 * and is never reused for the lifetime of the application.
	 *
	 * The ID value starts at 0 and increments from there. The value -1 is an invalid ID.
	 */
	typedef SDL_SensorID SensorID;

	/**
	 * Accelerometer sensor
	 *
	 * The accelerometer returns the current acceleration in SI meters per
	 * second squared. This measurement includes the force of gravity, so
	 * a device at rest will have an value of STANDARD_GRAVITY away
	 * from the center of the earth, which is a positive Y value.
	 *
	 * values[0]: Acceleration on the x axis
	 * values[1]: Acceleration on the y axis
	 * values[2]: Acceleration on the z axis
	 *
	 * For phones held in portrait mode and game controllers held in front of you,
	 * the axes are defined as follows:
	 * -X ... +X : left ... right
	 * -Y ... +Y : bottom ... top
	 * -Z ... +Z : farther ... closer
	 *
	 * The axis data is not changed when the phone is rotated.
	 */
	constexpr float STANDARD_GRAVITY = SDL_STANDARD_GRAVITY;

	struct Sensor
	{
		/* The different sensors defined by SDL
		 *
		 * Additional sensors may be available, using platform dependent semantics.
		 *
		 * Hare are the additional Android sensors:
		 * https://developer.android.com/reference/android/hardware/SensorEvent.html#values
		 */
		enum class Type
		{
			INVALID =  SDL_SENSOR_INVALID, /**< Returned for an invalid sensor */
			UNKNOWN =  SDL_SENSOR_UNKNOWN, /**< Unknown sensor type */
			ACCEL   =  SDL_SENSOR_ACCEL,   /**< Accelerometer */
			GYRO    =  SDL_SENSOR_GYRO,    /**< Gyroscope */
			ACCEL_L =  SDL_SENSOR_ACCEL_L, /**< Accelerometer for left Joy-Con controller and Wii nunchuk */
			GYRO_L  =  SDL_SENSOR_GYRO_L,  /**< Gyroscope for left Joy-Con controller */
			ACCEL_R =  SDL_SENSOR_ACCEL_R, /**< Accelerometer for right Joy-Con controller */
			GYRO_R  =  SDL_SENSOR_GYRO_R   /**< Gyroscope for right Joy-Con controller */
		};

		// This is custom destructor for smart pointers that destroys SDL_Sensors through SDL
		inline static void DestroySensor(SDL_Sensor* sensor)
			{ SDL_SensorClose(sensor); }

		// This is custom destructor for smart pointers that does not destroy the Sensor. This is for pointers you do not own
		inline static void DontDestroySensor(SDL_Sensor* sensor)
			{}

		// This creates a smart pointer to an SDL_Sensor with a custom destructor
		inline static std::shared_ptr<SDL_Sensor> MakeSharedPtr(SDL_Sensor* sensor)
			{ return std::shared_ptr<SDL_Sensor>(sensor, DestroySensor); }

		// This creates a Sensor from a SDL_Sensor pointer, taking ownership of the pointer
		inline static Sensor FromPtr(SDL_Sensor* sensor)
			{ return Sensor(MakeSharedPtr(sensor)); }

		// This creates a Sensor from a SDL_Sensor pointer, but does not take ownership of the pointer
		inline static Sensor FromUnownedPtr(SDL_Sensor* sensor)
			{ return Sensor(std::shared_ptr<SDL_Sensor>(sensor, DontDestroySensor)); }

		std::shared_ptr<SDL_Sensor> sensor = nullptr;

		inline Sensor(std::shared_ptr<SDL_Sensor> sensor)
			: sensor(sensor) {}

#if SDL_VERSION_ATLEAST(2, 0, 14)

		/**
		 * Locking for multi-threaded access to the sensor API
		 *
		 * If you are using the sensor API or handling events from multiple threads
		 * you should use these locking functions to protect access to the sensors.
		 *
		 * In particular, you are guaranteed that the sensor list won't change, so the
		 * API functions that take a sensor index will be valid, and sensor events
		 * will not be delivered.
		 */
		inline static void LockSensors  () { SDL_LockSensors  (); }
		inline static void UnlockSensors() { SDL_UnlockSensors(); }

#endif

		/**
		 * Count the number of sensors attached to the system right now.
		 *
		 * \returns the number of sensors detected.
		 */
		inline static int NumSensors()
			{ return SDL_NumSensors(); }

		/**
		 * Get the implementation dependent name of a sensor.
		 *
		 * \param device_index The sensor to obtain name from
		 * \returns the sensor name, or NULL if `device_index` is out of range.
		 */
		inline static const char* GetDeviceName(int device_index)
			{ return SDL_SensorGetDeviceName(device_index); }

		/**
		 * Get the type of a sensor.
		 *
		 * \param device_index The sensor to get the type from
		 * \returns the Sensor::Type, or `Type::INVALID` if `device_index` is
		 *          out of range.
		 */
		inline static Type GetDeviceType(int device_index)
			{ return (Type)SDL_SensorGetDeviceType(device_index); }

		/**
		 * Get the platform dependent type of a sensor.
		 *
		 * \param device_index The sensor to check
		 * \returns the sensor platform dependent type, or -1 if `device_index` is out
		 *          of range.
		 */
		inline static int GetDeviceNonPortableType(int device_index)
			{  return SDL_SensorGetDeviceNonPortableType(device_index); }

		/**
		 * Get the instance ID of a sensor.
		 *
		 * \param device_index The sensor to get instance id from
		 * \returns the sensor instance ID, or -1 if `device_index` is out of range.
		 */
		inline static SensorID GetDeviceInstanceID(int device_index)
			{ return SDL_SensorGetDeviceInstanceID(device_index); }

		/**
		 * Open a sensor for use.
		 *
		 * \param device_index The sensor to open
		 * \returns a Sensor sensor object, or NULL if an error occurred.
		 */
		inline static Sensor Open(int device_index)
			{ return FromPtr(SDL_SensorOpen(device_index)); }

		/**
		 * Return the Sensor associated with an instance id.
		 *
		 * \param instance_id The sensor from instance id
		 * \returns an SDL_Sensor object.
		 */
		inline static Sensor FromInstanceID(SensorID instance_id)
			{ return FromUnownedPtr(SDL_SensorFromInstanceID(instance_id)); }

		/**
		 * Get the implementation dependent name of a sensor
		 *
		 * \returns the sensor name, or NULL if `sensor` is NULL.
		 */
		inline const char* GetName()
			{ return SDL_SensorGetName(sensor.get()); }

		/**
		 * Get the type of a sensor.
		 *
		 * \returns the Sensor::Type type, or `Type::INVALID` if `sensor` is
		 *          NULL.
		 */
		inline Type GetType()
			{ return (Type)SDL_SensorGetType(sensor.get()); }

		/**
		 * Get the platform dependent type of a sensor.
		 *
		 * \returns the sensor platform dependent type, or -1 if `sensor` is NULL.
		 */
		inline int GetNonPortableType()
			{ return SDL_SensorGetNonPortableType(sensor.get()); }

		/**
		 * Get the instance ID of a sensor.
		 *
		 * \returns the sensor instance ID, or -1 if `sensor` is NULL.
		 */
		inline SensorID GetInstanceID()
			{ return SDL_SensorGetInstanceID(sensor.get()); }

		/**
		 * Get the current state of an opened sensor.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 *
		 * \param data A pointer filled with the current sensor state
		 * \param num_values The number of values to write to data
		 * \returns true or false if an error occurred.
		 */
		inline bool GetData(float* data, int num_values)
			{ return SDL_SensorGetData(sensor.get(), data, num_values) == 0; }

		/**
		 * Get the current state of an opened sensor.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 *
		 * \param data An array filled with the current sensor state
		 * \returns true or false if an error occurred.
		 */
		template <const int num_values>
		inline bool GetData(float(&data)[num_values])
			{ return SDL_SensorGetData(sensor.get(), data, num_values) == 0; }

		/**
		 * Get the current state of an opened sensor.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 *
		 * \param data A container filled with the current sensor state
		 * \returns true or false if an error occurred.
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<float, T>::is_continuous_container>>
		inline bool GetData(T& data)
			{ return SDL_SensorGetData(sensor.get(), data.data(), data.size()) == 0; }

#if SDL_VERSION_ATLEAST(2, 26, 0)

		/**
		 * Get the current state of an opened sensor with the timestamp of the last
		 * update.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 *
		 * \param timestamp A pointer filled with the timestamp in microseconds of the
		 *                  current sensor reading if available, or 0 if not
		 * \param data A pointer filled with the current sensor state
		 * \param num_values The number of values to write to data
		 * \returns true or false if an error occurred.
		 */
		inline bool GetDataWithTimestamp(Uint64* timestamp, float* data, int num_values)
			{ return SDL_SensorGetDataWithTimestamp(sensor.get(), timestamp, data, num_values) == 0; }
		inline bool GetDataWithTimestamp(Uint64& timestamp, float* data, int num_values)
			{ return SDL_SensorGetDataWithTimestamp(sensor.get(), &timestamp, data, num_values) == 0; }

		/**
		 * Get the current state of an opened sensor with the timestamp of the last
		 * update.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 *
		 * \param timestamp A reference filled with the timestamp in microseconds of the
		 *                  current sensor reading if available, or 0 if not
		 * \param data An array filled with the current sensor state
		 * \returns true or false if an error occurred.
		 */
		template <const int num_values>
		inline bool GetDataWithTimestamp(Uint64& timestamp, float(&data)[num_values])
			{ return SDL_SensorGetDataWithTimestamp(sensor.get(), &timestamp, data, num_values) == 0; }

		/**
		 * Get the current state of an opened sensor with the timestamp of the last
		 * update.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 *
		 * \param timestamp A reference filled with the timestamp in microseconds of the
		 *                  current sensor reading if available, or 0 if not
		 * \param data A container filled with the current sensor state
		 * \returns true or false if an error occurred.
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<float, T>::is_continuous_container>>
		inline bool GetDataWithTimestamp(Uint64& timestamp, T&data)
			{ return SDL_SensorGetDataWithTimestamp(sensor.get(), &timestamp, data.data(), (int)data.size()) == 0; }
#endif

		/**
		 * Update the current state of the open sensors.
		 *
		 * This is called automatically by the event loop if sensor events are
		 * enabled.
		 *
		 * This needs to be called from the thread that initialized the sensor
		 * subsystem.
		 */
		inline static void Update()
			{ SDL_SensorUpdate(); }
	};
}

#endif
#endif