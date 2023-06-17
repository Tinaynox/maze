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
#if (!defined(_MazeGeoLocation_hpp_))
#define _MazeGeoLocation_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    struct MAZE_CORE_API GeoLocationData
    {
        String name;
        S32 callingCode;
        String iso2;
        String iso3;
    };


    //////////////////////////////////////////
    enum class GeoLocation : U32
    {
        Unknown                                               = 0,
        AntiguaAndBarbuda                                     = 2,
        Afghanistan                                           = 3,
        Algeria                                               = 4,
        Azerbaijan                                            = 5,
        Albania                                               = 6,
        Armenia                                               = 7,
        Andorra                                               = 8,
        Angola                                                = 9,
        AmericanSamoa                                         = 10,
        Argentina                                             = 11,
        Australia                                             = 12,
        Austria                                               = 14,
        Bahrain                                               = 17,
        Barbados                                              = 18,
        Botswana                                              = 19,
        Bermuda                                               = 20,
        Belgium                                               = 21,
        Bahamas                                               = 22,
        Bangladesh                                            = 23,
        Belize                                                = 24,
        BosniaAndHerzegovina                                  = 25,
        Bolivia                                               = 26,
        Myanmar                                               = 27,
        Benin                                                 = 28,
        Belarus                                               = 29,
        SolomonIslands                                        = 30,
        Brazil                                                = 32,
        Bhutan                                                = 34,
        Bulgaria                                              = 35,
        Brunei                                                = 37,
        Burundi                                               = 38,
        Canada                                                = 39,
        Cambodia                                              = 40,
        Chad                                                  = 41,
        SriLanka                                              = 42,
        Congo                                                 = 43,
        CongoDRC                                              = 44,
        China                                                 = 45,
        Chile                                                 = 46,
        Cameroon                                              = 49,
        Comoros                                               = 50,
        Colombia                                              = 51,
        CostaRica                                             = 54,
        CentralAfricanRepublic                                = 55,
        Cuba                                                  = 56,
        CaboVerde                                             = 57,
        Cyprus                                                = 59,
        Denmark                                               = 61,
        Djibouti                                              = 62,
        Dominica                                              = 63,
        DominicanRepublic                                     = 65,
        Ecuador                                               = 66,
        Egypt                                                 = 67,
        Ireland                                               = 68,
        EquatorialGuinea                                      = 69,
        Estonia                                               = 70,
        Eritrea                                               = 71,
        ElSalvador                                            = 72,
        Ethiopia                                              = 73,
        CzechRepublic                                         = 75,
        Finland                                               = 77,
        Fiji                                                  = 78,
        Micronesia1                                           = 80,
        FaroeIslands                                          = 81,
        France                                                = 84,
        Gambia                                                = 86,
        Gabon                                                 = 87,
        Georgia                                               = 88,
        Ghana                                                 = 89,
        Gibraltar                                             = 90,
        Grenada                                               = 91,
        Greenland                                             = 93,
        Germany                                               = 94,
        Greece                                                = 98,
        Guatemala                                             = 99,
        Guinea                                                = 100,
        Guyana                                                = 101,
        Haiti                                                 = 103,
        HongKongSAR                                           = 104,
        Honduras                                              = 106,
        Croatia                                               = 108,
        Hungary                                               = 109,
        Iceland                                               = 110,
        Indonesia                                             = 111,
        India                                                 = 113,
        BritishIndianOceanTerritory                           = 114,
        Iran                                                  = 116,
        Israel                                                = 117,
        Italy                                                 = 118,
        CotedIvoire                                           = 119,
        Iraq                                                  = 121,
        Japan                                                 = 122,
        Jamaica                                               = 124,
        JanMayen                                              = 125,
        Jordan                                                = 126,
        JohnstonAtoll                                         = 127,
        Kenya                                                 = 129,
        Kyrgyzstan                                            = 130,
        NorthKorea                                            = 131,
        Kiribati                                              = 133,
        Korea                                                 = 134,
        Kuwait                                                = 136,
        Kazakhstan                                            = 137,
        Laos                                                  = 138,
        Lebanon                                               = 139,
        Latvia                                                = 140,
        Lithuania                                             = 141,
        Liberia                                               = 142,
        Slovakia                                              = 143,
        Liechtenstein                                         = 145,
        Lesotho                                               = 146,
        Luxembourg                                            = 147,
        Libya                                                 = 148,
        Madagascar                                            = 149,
        MacaoSAR                                              = 151,
        Moldova                                               = 152,
        Mongolia                                              = 154,
        Malawi                                                = 156,
        Mali                                                  = 157,
        Monaco                                                = 158,
        Morocco                                               = 159,
        Mauritius                                             = 160,
        Mauritania                                            = 162,
        Malta                                                 = 163,
        Oman                                                  = 164,
        Maldives                                              = 165,
        Mexico                                                = 166,
        Malaysia                                              = 167,
        Mozambique                                            = 168,
        Niger                                                 = 173,
        Vanuatu                                               = 174,
        Nigeria                                               = 175,
        Netherlands                                           = 176,
        Norway                                                = 177,
        Nepal                                                 = 178,
        Nauru                                                 = 180,
        Suriname                                              = 181,
        Nicaragua                                             = 182,
        NewZealand                                            = 183,
        PalestinianAuthority                                  = 184,
        Paraguay                                              = 185,
        Peru                                                  = 187,
        Pakistan                                              = 190,
        Poland                                                = 191,
        Panama                                                = 192,
        Portugal                                              = 193,
        PapuaNewGuinea                                        = 194,
        Palau                                                 = 195,
        GuineaBissau                                          = 196,
        Qatar                                                 = 197,
        Reunion                                               = 198,
        MarshallIslands                                       = 199,
        Romania                                               = 200,
        Philippines                                           = 201,
        PuertoRico                                            = 202,
        Russia                                                = 203,
        Rwanda                                                = 204,
        SaudiArabia                                           = 205,
        SaintPierreAndMiquelon                                = 206,
        SaintKittsAndNevis                                    = 207,
        Seychelles                                            = 208,
        SouthAfrica                                           = 209,
        Senegal                                               = 210,
        Slovenia                                              = 212,
        SierraLeone                                           = 213,
        SanMarino                                             = 214,
        Singapore                                             = 215,
        Somalia                                               = 216,
        Spain                                                 = 217,
        SaintLucia                                            = 218,
        Sudan                                                 = 219,
        Svalbard                                              = 220,
        Sweden                                                = 221,
        Syria                                                 = 222,
        Switzerland                                           = 223,
        UnitedArabEmirates                                    = 224,
        TrinidadAndTobago                                     = 225,
        Thailand                                              = 227,
        Tajikistan                                            = 228,
        Tonga                                                 = 231,
        Togo                                                  = 232,
        SaoTomeAndPrincipe                                    = 233,
        Tunisia                                               = 234,
        Turkey                                                = 235,
        Tuvalu                                                = 236,
        Taiwan                                                = 237,
        Turkmenistan                                          = 238,
        Tanzania                                              = 239,
        Uganda                                                = 240,
        Ukraine                                               = 241,
        UnitedKingdom                                         = 242,
        UnitedStates                                          = 244,
        BurkinaFaso                                           = 245,
        Uruguay                                               = 246,
        Uzbekistan                                            = 247,
        SaintVincentAndtheGrenadines                          = 248,
        Venezuela                                             = 249,
        Vietnam                                               = 251,
        USVirginIslands                                       = 252,
        VaticanCity                                           = 253,
        Namibia                                               = 254,
        WakeIsland                                            = 258,
        Samoa                                                 = 259,
        Swaziland                                             = 260,
        Yemen                                                 = 261,
        Zambia                                                = 263,
        Zimbabwe                                              = 264,
        SerbiaAndMontenegroFormer                             = 269,
        Montenegro                                            = 270,
        Serbia                                                = 271,
        Curacao                                               = 273,
        Anguilla                                              = 300,
        SouthSudan                                            = 276,
        Antarctica                                            = 301,
        Aruba                                                 = 302,
        AscensionIsland                                       = 303,
        AshmoreAndCartierIslands                              = 304,
        BakerIsland                                           = 305,
        BouvetIsland                                          = 306,
        CaymanIslands                                         = 307,
        ChannelIslands                                        = 308,
        ChristmasIsland                                       = 309,
        ClippertonIsland                                      = 310,
        CocosKeelingIslands                                   = 311,
        CookIslands                                           = 312,
        CoralSeaIslands                                       = 313,
        DiegoGarcia                                           = 314,
        FalklandIslands                                       = 315,
        FrenchGuiana                                          = 317,
        FrenchPolynesia                                       = 318,
        FrenchSouthernTerritories                             = 319,
        Guadeloupe                                            = 321,
        Guam                                                  = 322,
        GuantanamoBay                                         = 323,
        Guernsey                                              = 324,
        HeardIslandAndMcDonaldIslands                         = 325,
        HowlandIsland                                         = 326,
        JarvisIsland                                          = 327,
        Jersey                                                = 328,
        KingmanReef                                           = 329,
        Martinique                                            = 330,
        Mayotte                                               = 331,
        Montserrat                                            = 332,
        NetherlandsAntillesFormer                             = 333,
        NewCaledonia                                          = 334,
        Niue                                                  = 335,
        NorfolkIsland                                         = 336,
        NorthernMarianaIslands                                = 337,
        PalmyraAtoll                                          = 338,
        PitcairnIslands                                       = 339,
        RotaIsland                                            = 340,
        Saipan                                                = 341,
        SouthGeorgiaAndtheSouthSandwichIslands                = 342,
        StHelenaAscensionAndTristandaCunha                    = 343,
        TinianIsland                                          = 346,
        Tokelau                                               = 347,
        TristandaCunha                                        = 348,
        TurksAndCaicosIslands                                 = 349,
        BritishVirginIslands                                  = 351,
        WallisAndFutuna                                       = 352,
        Africa                                                = 742,
        Asia                                                  = 2129,
        Europe                                                = 10541,
        IsleofMan                                             = 15126,
        MacedoniaFYRO                                         = 19618,
        Melanesia                                             = 20900,
        Micronesia2                                           = 21206,
        MidwayIslands                                         = 21242,
        NorthernAmerica                                       = 23581,
        Polynesia                                             = 26286,
        CentralAmerica                                        = 27082,
        Oceania                                               = 27114,
        SintMaarten                                           = 30967,
        SouthAmerica                                          = 31396,
        SaintMartin                                           = 31706,
        World                                                 = 39070,
        WesternAfrica                                         = 42483,
        MiddleAfrica                                          = 42484,
        NorthernAfrica                                        = 42487,
        CentralAsia                                           = 47590,
        SouthEasternAsia                                      = 47599,
        EasternAsia                                           = 47600,
        EasternAfrica                                         = 47603,
        EasternEurope                                         = 47609,
        SouthernEurope                                        = 47610,
        MiddleEast                                            = 47611,
        SouthernAsia                                          = 47614,
        TimorLeste                                            = 7299303,
        Kosovo                                                = 9914689,
        Americas                                              = 10026358,
        AlandIslands                                          = 10028789,
        Caribbean                                             = 10039880,
        NorthernEurope                                        = 10039882,
        SouthernAfrica                                        = 10039883,
        WesternEurope                                         = 10210824,
        AustraliaandNewZealand                                = 10210825,
        SaintBarthelemy                                       = 161832015,
        USMinorOutlyingIslands                                = 161832256,
        LatinAmericaandtheCaribbean                           = 161832257,
        BonaireSintEustatiusandSaba                           = 161832258
    };

    //////////////////////////////////////////
    String const& GeoLocationToString(GeoLocation _location);
    
    //////////////////////////////////////////
    GeoLocation StringToGeoLocation(String const& _str);

    //////////////////////////////////////////
    S32 GeoLocationToCallingCode(GeoLocation _location);

    //////////////////////////////////////////
    GeoLocation CallingCodeToGeoLocation(S32 _code);

    //////////////////////////////////////////
    String const& GeoLocationToISO2(GeoLocation _location);

    //////////////////////////////////////////
    GeoLocation ISO2ToGeoLocation(String const& _code);

    //////////////////////////////////////////
    String const& GeoLocationToISO3(GeoLocation _location);

    //////////////////////////////////////////
    GeoLocation ISO3ToGeoLocation(String const& _code);



    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(GeoLocation const& _value, String& _data)
    {
        _data = GeoLocationToString(_value);
    }

    //////////////////////////////////////////
    inline void ValueFromString(GeoLocation& _value, CString _data, Size _count)
    {
        _value = StringToGeoLocation(String(_data, _count));
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(GeoLocation const& _value)
    {
        return sizeof(GeoLocation);
    }

    //////////////////////////////////////////
    inline void SerializeValue(GeoLocation const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(GeoLocation));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(GeoLocation& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(GeoLocation));
    }
    
    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeGeoLocation_hpp_
//////////////////////////////////////////
