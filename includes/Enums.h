#pragma once

enum class CardType
{
    Feedingfrenzy,
    Mistform,
    Ambush,
    Baptism_of_blood,
    Beastform,
    Dash,
    Exploit,
    Look_into_my_eyes,
    Prey_upon,
    Ravening_Seduction,
    Thirst_for_sustenance,
    Feint,

    Administer_Aid,
    Confirm_Suspicion,
    Counterpunch,
    Deduce_Strategy,
    Education_Never_Ends,
    Elementary,
    Eliminate_The_Impossible,
    Feint2,
    Fixed_Point_in_a_Changing_Age,
    Master_of_Disguise,
    The_Game_Is_Afoot,
    Service_Revolver,
    Study_Methods,

};

enum class CardTiming
{
    During,
    Before,
    After,
    Immediate,
};

enum class NeedInput // کارت هایی که نیاز به کنترلر دارند
{
    None,
    SelectCardToDiscard,            // دست حریف رو نشون داده میشه تا بازیکن یکی از کارت ها را انتخاب و این کارت حذف شود
    SelectOwnCardToDiscard,         // دست بازیکنی که دراکولا دارد نشان داده میشود تا بازیکن هر تعداد از انها را حذف کند و به مقدار کارت هیولا اضافه میشود
    MoveSherlock3,                  // شرلوک سه خانه جابجا میشود
    ShowOpponentHand,               // دست حریف فقط نشون داده میشه، بدون انتخاب
    PlaceSisterInDraculaZone,       // اگر خواهری مرده بود اون رو به ناحیه ای ازناحیه دراکولا برمیگرداند
    PlaceDraculaAnywhere,           // بازیکن دراکولا را در هر خانه ای میتواند قرار دهد
    MoveOpponentFighter,            // جابجا کردن مبارز ان حریف ، شرلوک و واتسون تا دو خانه
    PlaceDraculaAdjacentToOpponent, // دراکولا رو کنار مبارز حریف بذار بازیکن انتخاب میکند یم خانه را
    MoveOwnFighter3,                // مبارز خود را میتواند تا سه خانه جابجا کند
};

enum class e_Menu
{
    Play,
    Exit,
    Help,
};