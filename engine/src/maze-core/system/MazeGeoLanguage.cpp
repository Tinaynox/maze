//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeCoreHeader.hpp"
#include "maze-core/system/MazeGeoLanguage.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static Map<GeoLanguage, GeoLanguageData> const c_GeoLanguages = 
    {
        std::make_pair(GeoLanguage::Afrikaans, GeoLanguageData{ "Afrikaans", "af", {1078} }),
        std::make_pair(GeoLanguage::Amharic, GeoLanguageData{ "Amharic", "am", {1118} }),
        std::make_pair(GeoLanguage::Arabic, GeoLanguageData{ "Arabic", "ar", {5121, 15361, 3073, 2049, 11265, 13313, 12289, 4097, 6145, 8193, 16385, 1025, 10241, 7169, 14337, 9217} }),
        std::make_pair(GeoLanguage::Mapudungun, GeoLanguageData{ "Mapudungun", "arn" }),
        std::make_pair(GeoLanguage::Assamese, GeoLanguageData{ "Assamese", "as" }),
        std::make_pair(GeoLanguage::Azerbaijani, GeoLanguageData{ "Azerbaijani", "az" }),
        std::make_pair(GeoLanguage::Bashkir, GeoLanguageData{ "Bashkir", "ba" }),
        std::make_pair(GeoLanguage::Belarusian, GeoLanguageData{ "Belarusian", "be", {1059} }),
        std::make_pair(GeoLanguage::Bulgarian, GeoLanguageData{ "Bulgarian", "bg" }),
        std::make_pair(GeoLanguage::Bengali, GeoLanguageData{ "Bengali", "bn" }),
        std::make_pair(GeoLanguage::Tibetan, GeoLanguageData{ "Tibetan", "bo" }),
        std::make_pair(GeoLanguage::Breton, GeoLanguageData{ "Breton", "br" }),
        std::make_pair(GeoLanguage::Bosnian, GeoLanguageData{ "Bosnian", "bs" }),
        std::make_pair(GeoLanguage::Catalan, GeoLanguageData{ "Catalan", "ca" }),
        std::make_pair(GeoLanguage::Corsican, GeoLanguageData{ "Corsican", "co" }),
        std::make_pair(GeoLanguage::Czech, GeoLanguageData{ "Czech", "cs" }),
        std::make_pair(GeoLanguage::Welsh, GeoLanguageData{ "Welsh", "cy" }),
        std::make_pair(GeoLanguage::Danish, GeoLanguageData{ "Danish", "da" }),
        std::make_pair(GeoLanguage::German, GeoLanguageData{ "German", "de" }),
        std::make_pair(GeoLanguage::LowerSorbian, GeoLanguageData{ "LowerSorbian", "dsb" }),
        std::make_pair(GeoLanguage::Divehi, GeoLanguageData{ "Divehi", "dv" }),
        std::make_pair(GeoLanguage::Greek, GeoLanguageData{ "Greek", "el" }),
        std::make_pair(GeoLanguage::English, GeoLanguageData{ "English", "en", {3081, 10249, 4105, 9225, 2057, 16393, 6153, 8201, 5129, 13321, 7177, 11273, 1033, 12297} }),
        std::make_pair(GeoLanguage::Spanish, GeoLanguageData{ "Spanish", "es" }),
        std::make_pair(GeoLanguage::Estonian, GeoLanguageData{ "Estonian", "et" }),
        std::make_pair(GeoLanguage::Basque, GeoLanguageData{ "Basque", "eu" }),
        std::make_pair(GeoLanguage::Persian, GeoLanguageData{ "Persian", "fa" }),
        std::make_pair(GeoLanguage::Finnish, GeoLanguageData{ "Finnish", "fi" }),
        std::make_pair(GeoLanguage::Filipino, GeoLanguageData{ "Filipino", "fil" }),
        std::make_pair(GeoLanguage::Faroese, GeoLanguageData{ "Faroese", "fo" }),
        std::make_pair(GeoLanguage::French, GeoLanguageData{ "French", "fr" }),
        std::make_pair(GeoLanguage::Frisian, GeoLanguageData{ "Frisian", "fy" }),
        std::make_pair(GeoLanguage::Irish, GeoLanguageData{ "Irish", "ga" }),
        std::make_pair(GeoLanguage::ScottishGaelic, GeoLanguageData{ "ScottishGaelic", "gd" }),
        std::make_pair(GeoLanguage::Galician, GeoLanguageData{ "Galician", "gl" }),
        std::make_pair(GeoLanguage::SwissGerman, GeoLanguageData{ "SwissGerman", "gsw" }),
        std::make_pair(GeoLanguage::Gujarati, GeoLanguageData{ "Gujarati", "gu" }),
        std::make_pair(GeoLanguage::Hausa, GeoLanguageData{ "Hausa", "ha" }),
        std::make_pair(GeoLanguage::Hebrew, GeoLanguageData{ "Hebrew", "he" }),
        std::make_pair(GeoLanguage::Hindi, GeoLanguageData{ "Hindi", "hi" }),
        std::make_pair(GeoLanguage::Croatian, GeoLanguageData{ "Croatian", "hr" }),
        std::make_pair(GeoLanguage::UpperSorbian, GeoLanguageData{ "UpperSorbian", "hsb" }),
        std::make_pair(GeoLanguage::Hungarian, GeoLanguageData{ "Hungarian", "hu" }),
        std::make_pair(GeoLanguage::Armenian, GeoLanguageData{ "Armenian", "hy" }),
        std::make_pair(GeoLanguage::Indonesian, GeoLanguageData{ "Indonesian", "id" }),
        std::make_pair(GeoLanguage::Igbo, GeoLanguageData{ "Igbo", "ig" }),
        std::make_pair(GeoLanguage::Yi, GeoLanguageData{ "Yi", "ii" }),
        std::make_pair(GeoLanguage::Icelandic, GeoLanguageData{ "Icelandic", "is" }),
        std::make_pair(GeoLanguage::Italian, GeoLanguageData{ "Italian", "it" }),
        std::make_pair(GeoLanguage::Inuktitut, GeoLanguageData{ "Inuktitut", "iu" }),
        std::make_pair(GeoLanguage::Japanese, GeoLanguageData{ "Japanese", "ja" }),
        std::make_pair(GeoLanguage::Georgian, GeoLanguageData{ "Georgian", "ka" }),
        std::make_pair(GeoLanguage::Kazakh, GeoLanguageData{ "Kazakh", "kk" }),
        std::make_pair(GeoLanguage::Greenlandic, GeoLanguageData{ "Greenlandic", "kl" }),
        std::make_pair(GeoLanguage::Khmer, GeoLanguageData{ "Khmer", "km" }),
        std::make_pair(GeoLanguage::Kannada, GeoLanguageData{ "Kannada", "kn" }),
        std::make_pair(GeoLanguage::Korean, GeoLanguageData{ "Korean", "ko" }),
        std::make_pair(GeoLanguage::Konkani, GeoLanguageData{ "Konkani", "kok" }),
        std::make_pair(GeoLanguage::Kyrgyz, GeoLanguageData{ "Kyrgyz", "ky" }),
        std::make_pair(GeoLanguage::Luxembourgish, GeoLanguageData{ "Luxembourgish", "lb" }),
        std::make_pair(GeoLanguage::Lao, GeoLanguageData{ "Lao", "lo" }),
        std::make_pair(GeoLanguage::Lithuanian, GeoLanguageData{ "Lithuanian", "lt" }),
        std::make_pair(GeoLanguage::Latvian, GeoLanguageData{ "Latvian", "lv" }),
        std::make_pair(GeoLanguage::Maori, GeoLanguageData{ "Maori", "mi" }),
        std::make_pair(GeoLanguage::Macedonian, GeoLanguageData{ "Macedonian", "mk" }),
        std::make_pair(GeoLanguage::Malayalam, GeoLanguageData{ "Malayalam", "ml" }),
        std::make_pair(GeoLanguage::Mongolian, GeoLanguageData{ "Mongolian", "mn" }),
        std::make_pair(GeoLanguage::Mohawk, GeoLanguageData{ "Mohawk", "moh" }),
        std::make_pair(GeoLanguage::Marathi, GeoLanguageData{ "Marathi", "mr" }),
        std::make_pair(GeoLanguage::Malay, GeoLanguageData{ "Malay", "ms" }),
        std::make_pair(GeoLanguage::Maltese, GeoLanguageData{ "Maltese", "mt" }),
        std::make_pair(GeoLanguage::Burmese, GeoLanguageData{ "Burmese", "my" }),
        std::make_pair(GeoLanguage::NorwegianBokmal, GeoLanguageData{ "Norwegian", "nb" }),
        std::make_pair(GeoLanguage::Nepali, GeoLanguageData{ "Nepali", "ne" }),
        std::make_pair(GeoLanguage::Dutch, GeoLanguageData{ "Dutch", "nl" }),
        std::make_pair(GeoLanguage::NorwegianNynorsk, GeoLanguageData{ "NorwegianNynorsk", "nn" }),
        std::make_pair(GeoLanguage::Norwegian, GeoLanguageData{ "Norwegian", "no" }),
        std::make_pair(GeoLanguage::Sesotho, GeoLanguageData{ "Sesotho", "st" }),
        std::make_pair(GeoLanguage::Occitan, GeoLanguageData{ "Occitan", "oc" }),
        std::make_pair(GeoLanguage::Odia, GeoLanguageData{ "Odia", "or" }),
        std::make_pair(GeoLanguage::Punjabi, GeoLanguageData{ "Punjabi", "pa" }),
        std::make_pair(GeoLanguage::Polish, GeoLanguageData{ "Polish", "pl" }),
        std::make_pair(GeoLanguage::Dari, GeoLanguageData{ "Dari", "prs" }),
        std::make_pair(GeoLanguage::Pashto, GeoLanguageData{ "Pashto", "ps" }),
        std::make_pair(GeoLanguage::Portuguese, GeoLanguageData{ "Portuguese", "pt" }),
        std::make_pair(GeoLanguage::Kiche, GeoLanguageData{ "K'iche", "quc" }),
        std::make_pair(GeoLanguage::Quechua, GeoLanguageData{ "Quechua", "qu" }),
        std::make_pair(GeoLanguage::Romansh, GeoLanguageData{ "Romansh", "rm" }),
        std::make_pair(GeoLanguage::Romanian, GeoLanguageData{ "Romanian", "ro" }),
        std::make_pair(GeoLanguage::Russian, GeoLanguageData{ "Russian", "ru", {1049, 2073} }),
        std::make_pair(GeoLanguage::Kinyarwanda, GeoLanguageData{ "Kinyarwanda", "rw" }),
        std::make_pair(GeoLanguage::Sanskrit, GeoLanguageData{ "Sanskrit", "sa" }),
        std::make_pair(GeoLanguage::Yakut, GeoLanguageData{ "Yakut", "sah" }),
        std::make_pair(GeoLanguage::SamiNorthern, GeoLanguageData{ "SamiNorthern", "se" }),
        std::make_pair(GeoLanguage::Sinhala, GeoLanguageData{ "Sinhala", "si" }),
        std::make_pair(GeoLanguage::Slovak, GeoLanguageData{ "Slovak", "sk" }),
        std::make_pair(GeoLanguage::Slovenian, GeoLanguageData{ "Slovenian", "sl" }),
        std::make_pair(GeoLanguage::SamiSouthern, GeoLanguageData{ "SamiSouthern", "sma" }),
        std::make_pair(GeoLanguage::SamiLule, GeoLanguageData{ "SamiLule", "smj" }),
        std::make_pair(GeoLanguage::SamiInari, GeoLanguageData{ "SamiInari", "smn" }),
        std::make_pair(GeoLanguage::SamiSkolt, GeoLanguageData{ "SamiSkolt", "sms" }),
        std::make_pair(GeoLanguage::Albanian, GeoLanguageData{ "Albanian", "sq" }),
        std::make_pair(GeoLanguage::Serbian, GeoLanguageData{ "Serbian", "sr" }),
        std::make_pair(GeoLanguage::Swedish, GeoLanguageData{ "Swedish", "sv" }),
        std::make_pair(GeoLanguage::Kiswahili, GeoLanguageData{ "Kiswahili", "sw" }),
        std::make_pair(GeoLanguage::Syriac, GeoLanguageData{ "Syriac", "syc" }),
        std::make_pair(GeoLanguage::Tamil, GeoLanguageData{ "Tamil", "ta" }),
        std::make_pair(GeoLanguage::Telugu, GeoLanguageData{ "Telugu", "te" }),
        std::make_pair(GeoLanguage::Tajik, GeoLanguageData{ "Tajik", "tg" }),
        std::make_pair(GeoLanguage::Thai, GeoLanguageData{ "Thai", "th" }),
        std::make_pair(GeoLanguage::Turkmen, GeoLanguageData{ "Turkmen", "tk" }),
        std::make_pair(GeoLanguage::Tswana, GeoLanguageData{ "Tswana", "tn" }),
        std::make_pair(GeoLanguage::Turkish, GeoLanguageData{ "Turkish", "tr" }),
        std::make_pair(GeoLanguage::Tatar, GeoLanguageData{ "Tatar", "tt" }),
        std::make_pair(GeoLanguage::Tamazight, GeoLanguageData{ "Tamazight", "tzm" }),
        std::make_pair(GeoLanguage::Uyghur, GeoLanguageData{ "Uyghur", "ug" }),
        std::make_pair(GeoLanguage::Ukrainian, GeoLanguageData{ "Ukrainian", "uk", {1058} }),
        std::make_pair(GeoLanguage::Urdu, GeoLanguageData{ "Urdu", "ur" }),
        std::make_pair(GeoLanguage::Uzbek, GeoLanguageData{ "Uzbek", "uz" }),
        std::make_pair(GeoLanguage::Vietnamese, GeoLanguageData{ "Vietnamese", "vi" }),
        std::make_pair(GeoLanguage::Wolof, GeoLanguageData{ "Wolof", "wo" }),
        std::make_pair(GeoLanguage::Xhosa, GeoLanguageData{ "Xhosa", "xh" }),
        std::make_pair(GeoLanguage::Yoruba, GeoLanguageData{ "Yoruba", "yo" }),
        std::make_pair(GeoLanguage::Chinese, GeoLanguageData{ "Chinese", "zh" }),
        std::make_pair(GeoLanguage::Zulu, GeoLanguageData{ "Zulu", "zu" })
    };

    //////////////////////////////////////////
    String const& GeoLanguageToString(GeoLanguage _location)
    {
        static String const nullString = "Unknown";

        Map<GeoLanguage, GeoLanguageData>::const_iterator it = c_GeoLanguages.find(_location);
        if (it != c_GeoLanguages.end())
            return it->second.name;

        return nullString;
    }
    
    //////////////////////////////////////////
    GeoLanguage StringToGeoLanguage(String const& _str)
    {
        for (Map<GeoLanguage, GeoLanguageData>::const_iterator  it = c_GeoLanguages.begin(),
                                                                end = c_GeoLanguages.end();
                                                                it != end;
                                                                ++it)
        {
            if (it->second.name == _str)
                return it->first;
        }

        return GeoLanguage::Unknown;
    }

    //////////////////////////////////////////
    String const& GeoLanguageToIETFTag(GeoLanguage _location)
    {
        static String const nullString = "";

        Map<GeoLanguage, GeoLanguageData>::const_iterator it = c_GeoLanguages.find(_location);
        if (it != c_GeoLanguages.end())
            return it->second.ietfTag;

        return nullString;
    }
    
    //////////////////////////////////////////
    GeoLanguage IETFTagToGeoLanguage(String const& _str)
    {
        for (Map<GeoLanguage, GeoLanguageData>::const_iterator  it = c_GeoLanguages.begin(),
                                                                end = c_GeoLanguages.end();
                                                                it != end;
                                                                ++it)
        {
            if (it->second.ietfTag == _str)
                return it->first;
        }

        return GeoLanguage::Unknown;
    }

    //////////////////////////////////////////
    GeoLanguage LCIDToGeoLanguage(S32 _value)
    {
        for (Map<GeoLanguage, GeoLanguageData>::const_iterator  it = c_GeoLanguages.begin(),
            end = c_GeoLanguages.end();
            it != end;
            ++it)
        {
            if (it->second.lcidCodes.count(_value))
                return it->first;
        }

        return GeoLanguage::Unknown;
    }

} // namespace Maze
//////////////////////////////////////////
