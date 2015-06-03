using System;
using System.Runtime.InteropServices;
using System.Security;
using System.Security.Permissions;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Threading;

namespace TestUI
{
    /// <summary>
    /// Parts of this code are derived from the SilverlightViewport.WPF project, which can be found at
    /// http://silverlightviewport.codeplex.com/
    /// Credit for the InteropBitmap operations go to them.
    /// </summary>
    internal class InteropImage : Image, IDisposable
    {
        #region P/Invoke
        [SuppressUnmanagedCodeSecurity]
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr CreateFileMapping(IntPtr hFile,
                                               IntPtr lpFileMappingAttributes,
                                               uint flProtect,
                                               uint dwMaximumSizeHigh,
                                               uint dwMaximumSizeLow,
                                               string lpName);

        [SuppressUnmanagedCodeSecurity]
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr MapViewOfFile(IntPtr hFileMappingObject,
                                           uint dwDesiredAccess,
                                           uint dwFileOffsetHigh,
                                           uint dwFileOffsetLow,
                                           uint dwNumberOfBytesToMap);

        [SuppressUnmanagedCodeSecurity]
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool UnmapViewOfFile(IntPtr lpBaseAddress);

        [SuppressUnmanagedCodeSecurity]
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool CloseHandle(IntPtr hHandle);

        private const uint FILE_MAP_ALL_ACCESS = 0xF001F;

        private const uint PAGE_READWRITE = 0x04;

        private readonly IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);
        #endregion

        /// <summary>
        /// Handle to the unmanaged bitmap source
        /// </summary>
        private InteropBitmap Bitmap;

        /// <summary>
        /// Pointer to the image buffer
        /// </summary>
        private IntPtr Buffer;

        /// <summary>
        /// Faked file map for an unswappable section of memory
        /// </summary>
        private IntPtr FileMap;

        new private ImageSource Source { get; set; }

        /// <summary>
        /// Creates a new interop image source for a WPF image to use
        /// </summary>
        public InteropImage()
        {
        }

        /// <summary>
        /// Constructs the unmanaged interop bitmap on startup
        /// </summary>
        /// <param name="Width">The width of the bitmap in pixels</param>
        /// <param name="Height">The height of the bitmap in pixels</param>
        [SecurityPermission(SecurityAction.Demand, UnmanagedCode = true)]
        public IntPtr InitializeRenderBitmap(int Width, int Height, int NumberOfChannels, int BitsPerChannel)
        {
            // Clean up the old buffer if there is one
            if (Buffer != IntPtr.Zero)
            {
                UnmapViewOfFile(Buffer);
                Buffer = IntPtr.Zero;
                CloseHandle(FileMap);
                FileMap = IntPtr.Zero;
            }

            // Determine the new pixel format
            PixelFormat pixelFormat = PixelFormats.Default;
            if(NumberOfChannels == 1)
            {
                if (BitsPerChannel == 8)
                {
                    pixelFormat = PixelFormats.Gray8;
                }
                else if (BitsPerChannel == 16)
                {
                    pixelFormat = PixelFormats.Gray16;
                }
            }
            else if(NumberOfChannels == 3)
            {
                if (BitsPerChannel == 8)
                {
                    pixelFormat = PixelFormats.Rgb24;
                }
                else if (BitsPerChannel == 16)
                {
                    pixelFormat = PixelFormats.Rgb48;
                }
            }
            if(pixelFormat == PixelFormats.Default)
            {
                throw new Exception("Pixel format couldn't be determined.");
            }

            // Create the InteropBitmap
            uint totalSize = (uint)(Width * Height * pixelFormat.BitsPerPixel);
            FileMap = CreateFileMapping(INVALID_HANDLE_VALUE, IntPtr.Zero, PAGE_READWRITE, 0, totalSize, null);
            Buffer = MapViewOfFile(FileMap, FILE_MAP_ALL_ACCESS, 0, 0, totalSize);
            Dispatcher.BeginInvoke((Action)(() =>
            {
                Bitmap = (InteropBitmap)Imaging.CreateBitmapSourceFromMemorySection(FileMap, Width, Height, pixelFormat, Width * NumberOfChannels, 0);
                base.Source = Bitmap;
            }));

            return Buffer;
        }

        /// <summary>
        /// Clean up the memory map and buffer on closing.
        /// </summary>
        public void Dispose()
        {
            if (Buffer != IntPtr.Zero)
            {
                UnmapViewOfFile(Buffer);
                Buffer = IntPtr.Zero;
                CloseHandle(FileMap);
                FileMap = IntPtr.Zero;
            }
        }

        /// <summary>
        /// Redraw the WPF image once the buffer has been updated
        /// </summary>
        public void UpdateImage()
        {
            Dispatcher.BeginInvoke((Action)(() =>
            {
                if (!Bitmap.IsFrozen)
                {
                    Bitmap.Invalidate();
                }
            }));
        }
    }
}
