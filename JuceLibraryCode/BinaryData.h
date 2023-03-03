/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   logo_svg;
    const int            logo_svgSize = 14003;

    extern const char*   logo_png;
    const int            logo_pngSize = 3879;

    extern const char*   RobotoThin_ttf;
    const int            RobotoThin_ttfSize = 168488;

    extern const char*   OpenSansLight_ttf;
    const int            OpenSansLight_ttfSize = 129756;

    extern const char*   OpenSansMedium_ttf;
    const int            OpenSansMedium_ttfSize = 129948;

    extern const char*   OpenSansRegular_ttf;
    const int            OpenSansRegular_ttfSize = 129796;

    extern const char*   OpenSansSemiBold_ttf;
    const int            OpenSansSemiBold_ttfSize = 129716;

    extern const char*   TitilliumWebBlack_ttf;
    const int            TitilliumWebBlack_ttfSize = 43664;

    extern const char*   TitilliumWebBold_ttf;
    const int            TitilliumWebBold_ttfSize = 53896;

    extern const char*   TitilliumWebBoldItalic_ttf;
    const int            TitilliumWebBoldItalic_ttfSize = 62924;

    extern const char*   TitilliumWebExtraLight_ttf;
    const int            TitilliumWebExtraLight_ttfSize = 56724;

    extern const char*   TitilliumWebExtraLightItalic_ttf;
    const int            TitilliumWebExtraLightItalic_ttfSize = 60848;

    extern const char*   TitilliumWebItalic_ttf;
    const int            TitilliumWebItalic_ttfSize = 65284;

    extern const char*   TitilliumWebLight_ttf;
    const int            TitilliumWebLight_ttfSize = 57600;

    extern const char*   TitilliumWebLightItalic_ttf;
    const int            TitilliumWebLightItalic_ttfSize = 64560;

    extern const char*   TitilliumWebRegular_ttf;
    const int            TitilliumWebRegular_ttfSize = 57392;

    extern const char*   TitilliumWebSemiBold_ttf;
    const int            TitilliumWebSemiBold_ttfSize = 56752;

    extern const char*   TitilliumWebSemiBoldItalic_ttf;
    const int            TitilliumWebSemiBoldItalic_ttfSize = 64816;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 18;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
