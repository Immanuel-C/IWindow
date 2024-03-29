#pragma once

#include <string>
#include <vector>
#include "IWindowCore.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define IWINDOW_CREATE_FLAGS_FROM_ENUM_STRUCT(Struct)																	            \
		inline Struct operator|(Struct left, Struct right) {														                \
			return (Struct)((int64_t)(left) | (int64_t)(right));																	\
		}																	                                                        \
		inline Struct operator&(Struct left, Struct right) {														                \
			return (Struct)((int64_t)(left) & (int64_t)(right));																	\
		}	                                                                                                                        \
        inline void operator|=(Struct& left, const Struct& right) {                                                           \
            left = left | right;																	                                \
        }                                                                                                                           \
        inline void operator&=(Struct& left, const Struct& right) {                                                           \
            left = left & right;																	                                \
        }

namespace IWindow {
    /// <summary>
    /// A Vector2 of value of type T.
    /// </summary>
    /// <typeparam name="T">What this vector will be holding. Must be a numeral type.</typeparam>
    template <typename T>
    struct Vector2 {
        union {
            T x, r, width;
        };

        union {
            T y, g, height;
        };

        /// <summary>
        /// Checks if both unions are equal to zero.
        /// </summary>
        inline bool IsEmpty() const {
            return x == 0 && y == 0;
        }

        inline bool operator==(const Vector2<T>& right) { return x == right.x && y == right.y; }
        inline bool operator!=(const Vector2<T>& right) { return x != right.x && y != right.y; }
    };

    /// <summary>
    /// A Vector3 of value of type T. 
    /// </summary>
    /// <typeparam name="T">What this vector will be holding. Must be a numeral type.</typeparam>
    template <typename T>
    struct Vector3 {
        union {
            T x, r, width;
        };

        union {
            T y, g, height;
        };

        union {
            T z, b, depth;
        };

        /// <summary>
        /// Checks if all the unions are equal to zero.
        /// </summary>
        inline bool IsEmpty() const { return x == 0 && y == 0 && z == 0; }

        inline bool operator==(const Vector3<T>& right) { return x == right.x && y == right.y && z == right.z; }
        inline bool operator!=(const Vector3<T>& right) { return x != right.x && y != right.y && z != right.z; }
    };

    /// A Vector4 of value of type T. 
    /// </summary>
    /// <typeparam name="T">What this vector will be holding. Must be a numeral type.</typeparam>
    template <typename T>
    struct Vector4 {
        union {
            T x, r, width;
        };

        union {
            T y, g, height;
        };

        union {
            T z, b, depth;
        };

        union {
            T w, a;
        };

        /// <summary>
        /// Checks if all the unions are equal to zero.
        /// </summary>
        inline bool IsEmpty() const { return x == 0 && y == 0 && z == 0 && w == 0; }

        inline bool operator==(const Vector3<T>& right) { return x == right.x && y == right.y && z == right.z && w == right.w; }
        inline bool operator!=(const Vector3<T>& right) { return x != right.x && y != right.y && z != right.z && w != right.w; }
    };

    /// <summary>
    /// size is a Vector2<int32_t> and is the width and height format of the image.
    /// data is a uint8_t buffer of image data in .bmp format.
    /// </summary>
    struct Image {
        Vector2<size_t> size;
        uint8_t* data;

        /// <summary>
        /// Checks if size is empty and data is equal to nullptr.
        /// </summary>
        inline bool IsEmpty() const {
            return size.IsEmpty() && data == nullptr;
        }
    };

    /// <summary>
    /// size is a Vector2<int32_t> of the width and height of the monitor in screen coordinates.
    /// position is a Vector2<int32_t> of the x and y coordinate of monitor in screen coordinates.
    /// name is the name the OS provides when querying all the monitors.
    /// dpi is the x and y dpi scale of this monitor.
    /// </summary>
    struct IWINDOW_API Monitor {
        Vector2<int32_t> size, position;
        std::string name;
        Vector2<uint32_t> dpi;

        /// <summary>
        /// Checks if size is empty, position is empty and name is empty.
        /// </summary>
        inline bool IsEmpty() const {
            return size.IsEmpty() && position.IsEmpty() && name.empty();
        }

        /// <returns>The primary monitors information.</returns>
        static Monitor GetPrimaryMonitor();
        /// <returns>A vector of all the monitors information.</returns>
        static std::vector<Monitor> GetAllMonitors();
    };

    /// <summary>
    /// A bitmask type.
    /// Customizes the window using the underlying OS's styles.
    /// </summary>
    enum struct Style {
        /// <summary>
        /// A default decorated window.
        /// </summary>
        Default = 0x1i32,
        /// <summary>
        /// Makes the window resizable. Does not apply in an undecorated window.
        /// </summary>
        Resizable = 0x2i32,
        /// <summary>
        /// Makes the window not resizable. Does not apply in an undecorated window.
        /// </summary>
        NotResizable = 0x4i32,
        /// <summary>
        /// Makes the window visible.
        /// </summary>
        Visible = 0x8i32,
        /// <summary>
        /// Make the window not visible.
        /// </summary>
        NotVisible = 0x10i32,
        /// <summary>
        /// Make the window decorated. Does not apply to fullscreen windows.
        /// </summary>
        Decorated = 0x20i32,
        /// <summary>
        /// Make the window not decorated. Does not apply to fullscreen windows.
        /// </summary>
        NotDecorated = 0x40i32,
        /// <summary>
        /// Maximize the window to fill the screen with decoration.
        /// </summary>
        Maximize = 0x80i32,
        /// <summary>
        /// Restores the window out of the maximize state.
        /// </summary>
        Restore = 0x100i32,
        /// <summary>
        /// Do not use.
        /// </summary>
        Max
    };
    IWINDOW_CREATE_FLAGS_FROM_ENUM_STRUCT(Style)


    /// <summary>
    /// Type of error returned by the error callback.
    /// </summary>
    enum struct ErrorType {
        WindowApi,
        Monitor,
        OpenGL,
        Vulkan,
    };

    /// <summary>
    /// Convert ErrorType into an std::string
    /// </summary>
    inline std::string ErrorTypeToString(const ErrorType& error) {
        switch (error)
        {
        case ErrorType::WindowApi:
            return "ErrorType::WindowApi";
        case ErrorType::OpenGL:
            return "ErrorType::OpenGL";
        case ErrorType::Vulkan:
            return "ErrorType::Vulkan";
        default:
            return "Invalid Enum";
        }
    }

    /// <summary>
    /// Severity returned by the error callback.
    /// </summary>
    enum struct ErrorSeverity {
        FatalError,
        Error,
        Warning,
        Info
    };

    /// <summary>
    /// Convert ErrorSeverity into an std::string
    /// </summary>
    inline std::string ErrorSeverityToString(const ErrorSeverity& severity) {
        switch (severity)
        {
        case ErrorSeverity::FatalError:
            return "ErrorSeverity::FatalError";
        case ErrorSeverity::Error:
            return "ErrorSeverity::Error";
        case ErrorSeverity::Warning:
            return "ErrorSeverity::Warning";
        case ErrorSeverity::Info:
            return "ErrorSeverity::Info";
        default:
            return "Invalid Enum";
        }
    }

    /// <summary>
    /// Struct error callback returns.
    /// </summary>
    struct Error {
        std::string message;
        ErrorType type;
        ErrorSeverity severity;
    };
}