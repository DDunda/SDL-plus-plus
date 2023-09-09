#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 18)
#ifndef SDL_hidapi_hpp_
#define SDL_hidapi_hpp_
#pragma once

#include <SDL_hidapi.h>

#include <memory>
#include <string>
#include <vector>

namespace SDL::HID
{
	namespace
	{
		template <typename t, typename T>
		struct ContinuousContainer_traits
		{
			template <typename T>
			static auto has_data(T x) -> decltype((t*)x.data(), std::true_type{});
			static auto has_data(...) -> std::false_type;

			template <typename T>
			static auto has_size(T x) -> decltype((size_t)x.size(), std::true_type{});
			static auto has_size(...) -> std::false_type;

			static constexpr bool is_continous_container = decltype(has_data(std::declval<T>()))::value && decltype(has_size(std::declval<T>()))::value;
		};
	}

	/**
	 * Initialize the HIDAPI library.
	 *
	 * This function initializes the HIDAPI library. Calling it is not strictly
	 * necessary, as it will be called automatically by HID::DeviceInfo() and any
	 * of the HID::Device() functions if it is needed. This function should be
	 * called at the beginning of execution however, if there is a chance of
	 * HIDAPI handles being opened by different threads simultaneously.
	 *
	 * Each call to this function should have a matching call to HID::Quit()
	 * 
	 * \returns true on success and false on error.
	 */
	inline bool Init()
		{ return SDL_hid_init() == 0; }

	/**
	 * Finalize the HIDAPI library.
	 *
	 * This function frees all of the static data associated with HIDAPI. It
	 * should be called at the end of execution to avoid memory leaks.
	 *
	 * \returns true on success and false on error.
	 */
	inline bool Quit()
		{ return SDL_hid_exit() == 0; }
	
	/**
	 * Check to see if devices may have been added or removed.
	 *
	 * Enumerating the HID devices is an expensive operation, so you can call this
	 * to see if there have been any system device changes since the last call to
	 * this function. A change in the counter returned doesn't necessarily mean
	 * that anything has changed, but you can make a new HID::DeviceInfoEnumerator
	 * to get an updated device list.
	 *
	 * Calling this function for the first time may cause a thread or other system
	 * resource to be allocated to track device change notifications.
	 *
	 * \returns a change counter that is incremented with each potential device
	 *          change, or 0 if device change detection isn't available.
	 */
	inline static Uint32 DeviceChangeCount()
		{ return SDL_hid_device_change_count(); }

	struct Device
	{
		typedef SDL_hid_device_info Info;

		struct InfoEnumerator
		{
			struct Iterator
			{
				using iterator_category = std::forward_iterator_tag;
				using difference_type = std::ptrdiff_t;
				using value_type = Info;
				using pointer = Info*;  // or also value_type*
				using reference = Info&;  // or also value_type&

				Iterator(std::shared_ptr<value_type> base, pointer ptr) : base(base), m_ptr(ptr) {}

				reference operator*() const { return *m_ptr; }
				pointer operator->() { return m_ptr; }

				// Prefix increment
				Iterator& operator++() { m_ptr = m_ptr->next; return *this; }

				// Postfix increment
				Iterator operator++(int) { Iterator tmp(base, m_ptr); ++(*this); return tmp; }

				friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
				friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

			private:
				pointer m_ptr;
				std::shared_ptr<value_type> base;
			};

			std::shared_ptr<Info> device_info = nullptr;

			// This is custom destructor for smart pointers that destroys SDL_hid_device_infos through SDL
			static void DestroyEnumerator(Info* device_info) { SDL_hid_free_enumeration(device_info); }

			// This is custom destructor for smart pointers that does not destroy the device. This is for pointers you do not own
			static void DontDestroyEnumerator(Info* device_info) {}

			// This creates a smart pointer to an SDL_hid_device_info with a custom destructor
			static std::shared_ptr<Info> MakeSharedPtr(Info* device_info) { return std::shared_ptr<Info>(device_info, DestroyEnumerator); }

			// This creates a device_info from a SDL_hid_device_info pointer, taking ownership of the pointer
			static InfoEnumerator FromPtr(Info* device_info) { return InfoEnumerator(MakeSharedPtr(device_info)); }

			// This creates a device_info from a SDL_hid_device_info pointer, but does not take ownership of the pointer
			static InfoEnumerator FromUnownedPtr(Info* device_info) { return InfoEnumerator(std::shared_ptr<Info>(device_info, DontDestroyEnumerator)); }

			InfoEnumerator(std::shared_ptr<Info> device_info) : device_info(device_info) {}

			/**
			 * Enumerate the HID Devices.
			 *
			 * This function returns a linked list of all the HID devices attached to the
			 * system which match vendor_id and product_id. If `vendor_id` is set to 0
			 * then any vendor matches. If `product_id` is set to 0 then any product
			 * matches. If `vendor_id` and `product_id` are both set to 0, then all HID
			 * devices will be returned.
			 *
			 * \param vendor_id The Vendor ID (VID) of the types of device to open.
			 * \param product_id The Product ID (PID) of the types of device to open.
			 * \returns A valid HID::Device::InfoEnumerator object on success or invalid
			 *          HID::Device::InfoEnumerator on failure.
			 */
			InfoEnumerator(unsigned short vendor_id, unsigned short product_id)
				: device_info(MakeSharedPtr(SDL_hid_enumerate(vendor_id, product_id))) {}


			/**
			 * Enumerate the HID Devices.
			 *
			 * This function returns a linked list of all the HID devices attached to the
			 * system;
			 *
			 * \returns A valid HID::Device::InfoEnumerator object on success or invalid
			 *          HID::Device::InfoEnumerator on failure.
			 */
			InfoEnumerator()
				: device_info(MakeSharedPtr(SDL_hid_enumerate(0, 0))) {}

			Iterator begin() { return Iterator(device_info, device_info.get()); }
			Iterator end() { return Iterator(device_info, NULL); }
		};

		std::shared_ptr<SDL_hid_device> device = nullptr;

		// This is custom destructor for smart pointers that destroys SDL_hid_devices through SDL
		static void DestroyDevice(SDL_hid_device* device) { SDL_hid_close(device); }

		// This is custom destructor for smart pointers that does not destroy the device. This is for pointers you do not own
		static void DontDestroyDevice(SDL_hid_device* device) {}

		// This creates a smart pointer to an SDL_hid_device with a custom destructor
		static std::shared_ptr<SDL_hid_device> MakeSharedPtr(SDL_hid_device* device) { return std::shared_ptr<SDL_hid_device>(device, DestroyDevice); }

		// This creates a device from a SDL_hid_device pointer, taking ownership of the pointer
		static Device FromPtr(SDL_hid_device* device) { return Device(MakeSharedPtr(device)); }

		// This creates a device from a SDL_hid_device pointer, but does not take ownership of the pointer
		static Device FromUnownedPtr(SDL_hid_device* device) { return Device(std::shared_ptr<SDL_hid_device>(device, DontDestroyDevice)); }

		Device(std::shared_ptr<SDL_hid_device> device = nullptr) : device(device) {}

		/**
		  * Open a HID device using a Vendor ID (VID), Product ID (PID) and optionally
		  * a serial number.
		  *
		  * If `serial_number` is NULL, the first device with the specified VID and
		  * PID is opened.
		  *
		  * \param vendor_id The Vendor ID (VID) of the device to open.
		  * \param product_id The Product ID (PID) of the device to open.
		  * \param serial_number The Serial Number of the device to open (Optionally
		  *                      NULL).
		  * \returns a valid HID::Device object on success or invalid HID::Device on
		  *          failure.
		  */
		Device(unsigned short vendor_id, unsigned short product_id, const wchar_t* serial_number)
			 : device(MakeSharedPtr(SDL_hid_open(vendor_id, product_id, serial_number))) {}
		/**
		  * Open a HID device using a Vendor ID (VID), Product ID (PID). The first
		  * device with the specified VID and PID is opened.
		  *
		  * \param vendor_id The Vendor ID (VID) of the device to open.
		  * \param product_id The Product ID (PID) of the device to open.
		  * \param serial_number The Serial Number of the device to open
		  * \returns a valid HID::Device object on success or invalid HID::Device on
		  *          failure.
		  */
		Device(unsigned short vendor_id, unsigned short product_id, const std::wstring& serial_number)
			: Device(vendor_id, product_id, serial_number.c_str()) {}
		/**
		  * Open a HID device using a Vendor ID (VID), Product ID (PID). The first
		  * device with the specified VID and PID is opened.
		  *
		  * \param vendor_id The Vendor ID (VID) of the device to open.
		  * \param product_id The Product ID (PID) of the device to open.
		  * \returns a valid HID::Device object on success or invalid HID::Device on
		  *          failure.
		  */
		Device(unsigned short vendor_id, unsigned short product_id)
			: Device(vendor_id, product_id, NULL) {}
		/**
		  * Open a HID device using a Device::Info struct.
		  *
		  * \param info The information of the device to open.
		  * \returns a valid HID::Device object on success or invalid HID::Device on
		  *          failure.
		  */
		Device(Info info)
			: Device(info.vendor_id, info.product_id, info.serial_number) {}

		/**
		 * Open a HID device by its path name.
		 *
		 * The path name be determined by calling HID::Enumerate(), or a
		 * platform-specific path name can be used (eg: /dev/hidraw0 on Linux).
		 *
		 * \param path The path name of the device to open
		 * \returns a valid HID::Device object on success or invalid HID::Device on
		 *          failure.
		 */
		Device(const char* path)
			: device(MakeSharedPtr(SDL_hid_open_path(path, false))) {}
		Device(const std::string& path)
			: Device(path.c_str()) {}

		/**
		 * Write an Output report to a HID device.
		 *
		 * The first byte of `data` must contain the Report ID. For devices which only
		 * support a single report, this must be set to 0x0. The remaining bytes
		 * contain the report data. Since the Report ID is mandatory, calls to
		 * HID::Write() will always contain one more byte than the report contains.
		 * For example, if a hid report is 16 bytes long, 17 bytes must be passed to
		 * HID::Write(), the Report ID (or 0x0, for devices with a single report),
		 * followed by the report data (16 bytes). In this example, the length passed
		 * in would be 17.
		 *
		 * HID::Write() will send the data on the first OUT endpoint, if one exists.
		 * If it does not, it will send the data through the Control Endpoint
		 * (Endpoint 0).
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param data The data to send, including the report number as the first
		 *             byte.
		 * \param length The length in bytes of the data to send.
		 * \returns the actual number of bytes written and -1 on error.
		 */
		inline int Write(const unsigned char* data, size_t length)
			{ return SDL_hid_write(device.get(), data, length); }

		/**
		 * Write an Output report to a HID device.
		 *
		 * The first byte of `data` must contain the Report ID. For devices which only
		 * support a single report, this must be set to 0x0. The remaining bytes
		 * contain the report data. Since the Report ID is mandatory, calls to
		 * HID::Write() will always contain one more byte than the report contains.
		 * For example, if a hid report is 16 bytes long, 17 bytes must be passed to
		 * HID::Write(), the Report ID (or 0x0, for devices with a single report),
		 * followed by the report data (16 bytes). In this example, the length passed
		 * in would be 17.
		 *
		 * HID::Write() will send the data on the first OUT endpoint, if one exists.
		 * If it does not, it will send the data through the Control Endpoint
		 * (Endpoint 0).
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param data The data to send, including the report number as the first
		 *             byte.
		 * \returns the actual number of bytes written and -1 on error.
		 */
		template<size_t length, typename = typename std::enable_if_t<length>=1>>
		 inline int Write(const unsigned char(&data)[length])
			{ return Write(data, length); }

		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<unsigned char, T>::is_continous_container>>
		inline int Write(const T& data)
			{ return Write(data.data(), data.size()); }

		/**
		 * Write an Output report to a HID device.
		 *
		 * The first byte of `data` must contain the Report ID. For devices which only
		 * support a single report, this must be set to 0x0. The remaining bytes
		 * contain the report data. Since the Report ID is mandatory, calls to
		 * HID::Write() will always contain one more byte than the report contains.
		 * For example, if a hid report is 16 bytes long, 17 bytes must be passed to
		 * HID::Write(), the Report ID (or 0x0, for devices with a single report),
		 * followed by the report data (16 bytes). In this example, the length passed
		 * in would be 17.
		 *
		 * HID::Write() will send the data on the first OUT endpoint, if one exists.
		 * If it does not, it will send the data through the Control Endpoint
		 * (Endpoint 0).
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param it An iterator to the data to send, including the report number as
		 *           the first byte.
		 * \param end An iterator that marks the end of the data to send.
		 * \returns the actual number of bytes written and -1 on error.
		 */
		template <typename T>
		inline int Write(T it, T end)
		{
			std::vector<unsigned char> values;
			for (; it != end; it++) { values.push_back(*it); }
			return Write(values.data(), values.size());
		}

		/**
		 * Read an Input report from a HID device with timeout.
		 *
		 * Input reports are returned to the host through the INTERRUPT IN endpoint.
		 * The first byte will contain the Report number if the device uses numbered
		 * reports.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param data A buffer to put the read data into.
		 * \param length The number of bytes to read. For devices with multiple
		 *               reports, make sure to read an extra byte for the report
		 *               number.
		 * \param milliseconds Timeout in milliseconds or -1 for blocking wait.
		 * \returns The actual number of bytes read and -1 on error. If no packet was
		 *          available to be read within the timeout period, this function
		 *          returns 0.
		 */
		inline int ReadTimeout(unsigned char* data, size_t length, int milliseconds)
			{ return SDL_hid_read_timeout(device.get(), data, length, milliseconds); }


		/**
		 * Read an Input report from a HID device with timeout.
		 *
		 * Input reports are returned to the host through the INTERRUPT IN endpoint.
		 * The first byte will contain the Report number if the device uses numbered
		 * reports.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param data A container to put the read data into.
		 * \param milliseconds Timeout in milliseconds or -1 for blocking wait.
		 * \returns The actual number of bytes read and -1 on error. If no packet was
		 *          available to be read within the timeout period, this function
		 *          returns 0.
		 */
		template<size_t length, typename = typename std::enable_if_t<length >= 1>>
		inline int ReadTimeout(unsigned char(&data)[length], int milliseconds)
			{ return ReadTimeout(data, length, milliseconds); }

		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<unsigned char, T>::is_continous_container>>
		inline int ReadTimeout(T& data, int milliseconds)
			{ return ReadTimeout(data.data(), data.size(), milliseconds); }

		/**
		 * Read an Input report from a HID device.
		 *
		 * Input reports are returned to the host through the INTERRUPT IN endpoint.
		 * The first byte will contain the Report number if the device uses numbered
		 * reports.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param data A buffer to put the read data into.
		 * \param length The number of bytes to read. For devices with multiple
		 *               reports, make sure to read an extra byte for the report
		 *               number.
		 * \returns the actual number of bytes read and -1 on error. If no packet was
		 *          available to be read and the handle is in non-blocking mode, this
		 *          function returns 0.
		 */
		int Read(unsigned char* data, size_t length)
			{ return SDL_hid_read(device.get(), data, length); }

		/**
		 * Read an Input report from a HID device.
		 *
		 * Input reports are returned to the host through the INTERRUPT IN endpoint.
		 * The first byte will contain the Report number if the device uses numbered
		 * reports.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param data A container to put the read data into.
		 * \returns the actual number of bytes read and -1 on error. If no packet was
		 *          available to be read and the handle is in non-blocking mode, this
		 *          function returns 0.
		 */
		template<size_t length, typename = typename std::enable_if_t<length >= 1>>
		inline int Read(unsigned char(&data)[length])
			{ return Read(data, length); }

		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<unsigned char, T>::is_continous_container>>
		inline int Read(T& data)
			{ return Read(data.data(), data.size()); }

		/**
		 * Set the device handle to be non-blocking.
		 *
		 * In non-blocking mode calls to HID::Read() will return immediately with a
		 * value of 0 if there is no data to be read. In blocking mode, HID::Read()
		 * will wait (block) until there is data to read before returning.
		 *
		 * Nonblocking can be turned on and off at any time.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param nonblock enable or not the nonblocking reads - true to enable
		 *                 nonblocking - false to disable nonblocking.
		 * \returns true on success and false on error.
		 */
		inline bool SetNonBlocking(bool nonblock)
			{ return SDL_hid_set_nonblocking(device.get(), nonblock ? 1 : 0) == 0; }

		/**
		 * Send a Feature report to the device.
		 *
		 * Feature reports are sent over the Control endpoint as a Set_Report
		 * transfer. The first byte of `data` must contain the Report ID. For devices
		 * which only support a single report, this must be set to 0x0. The remaining
		 * bytes contain the report data. Since the Report ID is mandatory, calls to
		 * HID::SendFeatureReport() will always contain one more byte than the report
		 * contains. For example, if a hid report is 16 bytes long, 17 bytes must be
		 * passed to HID::SendFeatureReport(): the Report ID (or 0x0, for devices 
		 * which do not use numbered reports), followed by the report data (16 bytes).
		 * In this example, the length passed in would be 17.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param data The data to send, including the report number as the first
		 *             byte.
		 * \param length The length in bytes of the data to send, including the report
		 *               number.
		 * \returns the actual number of bytes written and -1 on error.
		 */
		inline int SendFeatureReport(const unsigned char* data, size_t length)
			{ return SDL_hid_send_feature_report(device.get(), data, length); }

		/**
		 * Send a Feature report to the device.
		 *
		 * Feature reports are sent over the Control endpoint as a Set_Report
		 * transfer. The first byte of `data` must contain the Report ID. For devices
		 * which only support a single report, this must be set to 0x0. The remaining
		 * bytes contain the report data. Since the Report ID is mandatory, calls to
		 * HID::SendFeatureReport() will always contain one more byte than the report
		 * contains. For example, if a hid report is 16 bytes long, 17 bytes must be
		 * passed to HID::SendFeatureReport(): the Report ID (or 0x0, for devices 
		 * which do not use numbered reports), followed by the report data (16 bytes).
		 * In this example, the length passed in would be 17.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param data The data to send, including the report number as the first
		 *             byte.
		 * \returns the actual number of bytes written and -1 on error.
		 */
		template<size_t length, typename = typename std::enable_if_t<length>=1>>
		inline int SendFeatureReport(const unsigned char(&data)[length])
			{ return SendFeatureReport(data, length); }

		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<unsigned char, T>::is_continous_container>>
		inline int SendFeatureReport(const T& data)
			{ return SendFeatureReport(data.data(), data.size()); }

		/**
		 * Send a Feature report to the device.
		 *
		 * Feature reports are sent over the Control endpoint as a Set_Report
		 * transfer. The first byte of `data` must contain the Report ID. For devices
		 * which only support a single report, this must be set to 0x0. The remaining
		 * bytes contain the report data. Since the Report ID is mandatory, calls to
		 * HID::SendFeatureReport() will always contain one more byte than the report
		 * contains. For example, if a hid report is 16 bytes long, 17 bytes must be
		 * passed to HID::SendFeatureReport(): the Report ID (or 0x0, for devices
		 * which do not use numbered reports), followed by the report data (16 bytes).
		 * In this example, the length passed in would be 17.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param it An iterator to the data to send, including the report number as
		 *           the first byte.
		 * \param end An iterator that marks the end of the data to send.
		 * \returns the actual number of bytes written and -1 on error.
		 */
		template <typename T>
		inline int SendFeatureReport(T it, T end)
		{
			std::vector<unsigned char> values;
			for (; it != end; it++) { values.push_back(*it); }
			return SendFeatureReport(values.data(), values.size());
		}

		/**
		 * Get a feature report from a HID device.
		 *
		 * Set the first byte of `data` to the Report ID of the report to be read.
		 * Make sure to allow space for this extra byte in `data`. Upon return, the
		 * first byte will still contain the Report ID, and the report data will start
		 * in data[1].
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param data A buffer to put the read data into, including the Report ID.
		 *             Set the first byte of `data` to the Report ID of the report to
		 *             be read, or set it to zero if your device does not use numbered
		 *             reports.
		 * \param length The number of bytes to read, including an extra byte for the
		 *               report ID. The buffer can be longer than the actual report.
		 * \returns the number of bytes read plus one for the report ID (which is
		 *          still in the first byte), or -1 on error.
		 */
		inline int GetFeatureReport(unsigned char* data, size_t length)
			{ return SDL_hid_get_feature_report(device.get(), data, length); }

		/**
		 * Get a feature report from a HID device.
		 *
		 * Set the first byte of `data` to the Report ID of the report to be read.
		 * Make sure to allow space for this extra byte in `data`. Upon return, the
		 * first byte will still contain the Report ID, and the report data will start
		 * in data[1].
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param data A container to put the read data into, including the Report ID.
		 *             Set the first byte of `data` to the Report ID of the report to
		 *             be read, or set it to zero if your device does not use numbered
		 *             reports.
		 * \returns the number of bytes read plus one for the report ID (which is
		 *          still in the first byte), or -1 on error.
		 */
		template<size_t length, typename = typename std::enable_if_t<length >= 1>>
		inline int GetFeatureReport(unsigned char(&data)[length])
			{ return GetFeatureReport(data, length); }
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<unsigned char, T>::is_continous_container>>
		inline int GetFeatureReport(T& data)
			{ return GetFeatureReport(data.data(), data.size()); }

		/**
		 * Get The Manufacturer String from a HID device.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param string A wide string buffer to put the data into.
		 * \param maxlen The length of the buffer in multiples of wchar_t.
		 * \returns true on success and false on error.
		 */
		inline bool GetManufacturerString(wchar_t* string, size_t maxlen)
			{ return SDL_hid_get_manufacturer_string(device.get(), string, maxlen) == 0; }

		/**
		 * Get The Manufacturer String from a HID device.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param string A wide string buffer to put the data into.
		 * \returns true on success and false on error.
		 */
		template<size_t length>
		inline bool GetManufacturerString(wchar_t (&string)[length])
			{ return GetManufacturerString(string, length); }
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<wchar_t, T>::is_continous_container>>
		inline bool GetManufacturerString(T& string)
			{ return GetManufacturerString(string.data(), string.size()); }

		/**
		 * Get The Product String from a HID device.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param string A wide string buffer to put the data into.
		 * \returns true on success and false on error.
		 */
		inline bool GetProductString(wchar_t* string, size_t maxlen)
			{ return SDL_hid_get_product_string(device.get(), string, maxlen) == 0; }

		/**
		 * Get The Product String from a HID device.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param string A wide string buffer to put the data into.
		 * \returns true on success and false on error.
		 */
		template<size_t length>
		inline bool GetProductString(wchar_t (&string)[length])
			{ return GetProductString(string, length); }
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<wchar_t, T>::is_continous_container>>
		inline bool GetProductString(T& string)
			{ return GetProductString(string.data(), string.size()); }

		/**
		 * Get The Serial Number String from a HID device.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param string A wide string buffer to put the data into.
		 * \param maxlen The length of the buffer in multiples of wchar_t.
		 * \returns true on success and false on error.
		 */
		inline bool GetSerialNumberString(wchar_t* string, size_t maxlen)
			{ return SDL_hid_get_serial_number_string(device.get(), string, maxlen) == 0; }

		/**
		 * Get The Serial Number String from a HID device.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param string A wide string buffer to put the data into.
		 * \returns true on success and false on error.
		 */
		template<size_t length>
		inline bool GetSerialNumberString(wchar_t (&string)[length])
			{ return GetSerialNumberString(string, length); }
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<wchar_t, T>::is_continous_container>>
		inline bool GetSerialNumberString(T& string)
			{ return GetSerialNumberString(string.data(), string.size()); }

		/**
		 * Get a string from a HID device, based on its string index.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param string_index The index of the string to get.
		 * \param string A wide string buffer to put the data into.
		 * \param maxlen The length of the buffer in multiples of wchar_t.
		 * \returns true on success and false on error.
		 */
		inline bool GetIndexedString(int string_index, wchar_t* string, size_t maxlen)
			{ return SDL_hid_get_indexed_string(device.get(), string_index, string, maxlen) == 0; }

		/**
		 * Get a string from a HID device, based on its string index.
		 *
		 * \param device A device handle returned from HID::Open().
		 * \param string_index The index of the string to get.
		 * \param string A wide string buffer to put the data into.
		 * \returns true on success and false on error.
		 */
		template<size_t length>
		inline bool GetIndexedString(int string_index, wchar_t (&string)[length])
			{ return GetIndexedString(string_index, string, length); }
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<wchar_t, T>::is_continous_container>>
		inline bool GetIndexedString(int string_index, T& string)
			{ return GetIndexedString(string_index, string.data(), string.size()); }
	
	};

	/**
	 * Start or stop a BLE scan on iOS and tvOS to pair Steam Controllers
	 *
	 * \param active true to start the scan, false to stop the scan
	 */
	inline static void BLE_Scan(bool active) { SDL_hid_ble_scan(active ? SDL_TRUE : SDL_FALSE); }
}

#endif
#endif