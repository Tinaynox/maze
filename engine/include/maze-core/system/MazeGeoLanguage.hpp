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
#pragma once
#if (!defined(_MazeGeoLanguage_hpp_))
#define _MazeGeoLanguage_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    struct MAZE_CORE_API GeoLanguageData
    {
        String name;
        String ietfTag;
        Set<S32> localeCodes;
    };


    //////////////////////////////////////////
    enum class MAZE_CORE_API GeoLanguage : U8
    {
        Unknown                 = 0,
        Afrikaans,
        Amharic,
        Arabic,
        Mapudungun,
        Assamese,
        Azerbaijani,
        Bashkir,
        Belarusian,
        Bulgarian,
        Bengali,
        Tibetan,
        Breton,
        Bosnian,
        Catalan,
        Corsican,
        Czech,
        Welsh,
        Danish,
        German,
        LowerSorbian,
        Divehi,
        Greek,
        English,
        Spanish,
        Estonian,
        Basque,
        Persian,
        Finnish,
        Filipino,
        Faroese,
        French,
        Frisian,
        Irish,
        ScottishGaelic,
        Galician,
        SwissGerman,
        Gujarati,
        Hausa,
        Hebrew,
        Hindi,
        Croatian,
        UpperSorbian,
        Hungarian,
        Armenian,
        Indonesian,
        Igbo,
        Yi,
        Icelandic,
        Italian,
        Inuktitut,
        Japanese,
        Georgian,
        Kazakh,
        Greenlandic,
        Khmer,
        Kannada,
        Korean,
        Konkani,
        Kyrgyz,
        Luxembourgish,
        Lao,
        Lithuanian,
        Latvian,
        Maori,
        Macedonian,
        Malayalam,
        Mongolian,
        Mohawk,
        Marathi,
        Malay,
        Maltese,
        Burmese,
        NorwegianBokmal,
        Nepali,
        Dutch,
        NorwegianNynorsk,
        Norwegian,
        Sesotho,
        Occitan,
        Odia,
        Punjabi,
        Polish,
        Dari,
        Pashto,
        Portuguese,
        Kiche,
        Quechua,
        Romansh,
        Romanian,
        Russian,
        Kinyarwanda,
        Sanskrit,
        Yakut,
        SamiNorthern,
        Sinhala,
        Slovak,
        Slovenian,
        SamiSouthern,
        SamiLule,
        SamiInari,
        SamiSkolt,
        Albanian,
        Serbian,
        Swedish,
        Kiswahili,
        Syriac,
        Tamil,
        Telugu,
        Tajik,
        Thai,
        Turkmen,
        Tswana,
        Turkish,
        Tatar,
        Tamazight,
        Uyghur,
        Ukrainian,
        Urdu,
        Uzbek,
        Vietnamese,
        Wolof,
        Xhosa,
        Yoruba,
        Chinese,
        Zulu,
           
        MAX
    };

    //////////////////////////////////////////
    MAZE_CORE_API String const& GeoLanguageToString(GeoLanguage _location);
    
    //////////////////////////////////////////
    MAZE_CORE_API GeoLanguage StringToGeoLanguage(String const& _str);


    //////////////////////////////////////////
    MAZE_CORE_API String const& GeoLanguageToIETFTag(GeoLanguage _location);

    //////////////////////////////////////////
    MAZE_CORE_API GeoLanguage IETFTagToGeoLanguage(String const& _code);


    //////////////////////////////////////////
    MAZE_CORE_API GeoLanguage LocaleCodeToGeoLanguage(S32 _value);



    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(GeoLanguage const& _value, String& _data)
    {
        _data = GeoLanguageToString(_value);
    }

    //////////////////////////////////////////
    inline void ValueFromString(GeoLanguage& _value, CString _data, Size _count)
    {
        _value = StringToGeoLanguage(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(GeoLanguage const& _value)
    {
        return sizeof(GeoLanguage);
    }

    //////////////////////////////////////////
    inline void SerializeValue(GeoLanguage const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(GeoLanguage));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(GeoLanguage& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(GeoLanguage));
    }
    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeGeoLanguage_hpp_
//////////////////////////////////////////
