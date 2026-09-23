# 日本語文字列の移植訳案

`dat/strings.ja.json` のうち、旧フォークから判断・変更した 182 キーを確認するための一覧。
旧版と現行版で英語原文が同じ 883 キーは旧訳をそのまま使用した。
原文が空白のみのキーは、表示行の保持用として同じ空白を入れた。

## 原文が変更された 26 キー

| キー | 新しい英語 | 旧訳 | 訳案 |
|---|---|---|---|
| `SPLASH_SUBTITLE` | Dungeon Adventures inspired by Andrew Greenberg and Robert Woodhead | Andrew Greenberg と Robert Woodhead による Wizardry シリーズに着想を得たダンジョン冒険 | Andrew Greenberg と Robert Woodhead に着想を得たダンジョン冒険 |
| `TAVERN_INSPECT` | Inspect Roster | キャラクターを見る | 名簿を見る |
| `INN_TITLE` | Adventurer's Inn | 宿屋 | 冒険者の宿屋 |
| `PAY_COST_PREFIX` | THE DONATION WILL BE | 費用は | 寄付金は |
| `SHOP_TITLE` | Boltac's Trading Post | 商店 | ボルタックの交易所 |
| `REORDER_SUBTITLE` | Reorder: | 並び替え: | 並び替え: |
| `ROSTER_TITLE` | Roster of Characters | キャラクターを見る | キャラクター名簿 |
| `GAME_MESSAGE_AREA_OUT_OF_BOUNDS_2` |      ** AREA OUT OF BOUNDS! **        |       ** 立入禁止区域！ **        |       ** 立入禁止区域！ **        |
| `GAME_MESSAGE_MURPHYS_GHOSTS_2` |  hooded  man  here.  A  golden  light  |  かぶった男の像がある。フードからは  |   かぶった男の像がある。フードからは  |
| `GAME_MESSAGE_FROG_STATUE_4` | while it yells 'Yeah.. Yeah..'         | 「Yeah.. Yeah..」と叫ぶ。  | 「Yeah.. Yeah..」と叫ぶ。  |
| `GAME_MESSAGE_MONSTER_ALLOCATION_CENTRE_2` |      ''Monster Allocation Centre'      |       「モンスター配備センター」      |       「モンスター配備センター」      |
| `GAME_MESSAGE_LARGE_DESK_4` | broken,                                | あなたが部屋に入ると、扉の | どれも今は壊れている。 |
| `GAME_MESSAGE_LARGE_DESK_5` | door slammed shut.  Then it  glowed  a | パネルがバタンと閉まり、 | バタンと閉まり、淡い青色に光る。 |
| `GAME_MESSAGE_LARGE_DESK_6` | pale blue. No one can pry it open.     | 淡い青色に光り出す。誰もこじ開けられない。 | 誰にもこじ開けられない。 |
| `GAME_MESSAGE_LARGE_DESK_7` | Next,  the door on the other  side of  | 次に部屋の反対側の扉が | 続いて、部屋の反対側の扉が |
| `GAME_MESSAGE_LARGE_DESK_8` | the  room  starts  to   glow   bright  | 鮮やかなオレンジ色に光り始め、 | 鮮やかなオレンジ色に輝き始め、 |
| `GAME_MESSAGE_LARGE_DESK_9` | orange, inviting the party to use it.  | パーティを誘うように輝いている。 | パーティを誘うように光っている。 |
| `GAME_MESSAGE_WERDNA_BOAST_1_8` | you this clue:'                         | 汝らにこのヒントを与えよう: | 汝らにこのヒントを与えよう」 |
| `GAME_MESSAGE_WERDNA_BOAST_3_1` |            PS - TREBOX SUX!            |            追伸 - TREBOX SUX！」           |            追伸 - TREBOX SUX！            |
| `GAME_MESSAGE_THREE_HUMANOIDS_5` | （空白 38 文字） | バーバリアンは廊下の奥を指さして | （空白 38 文字） |
| `GAME_MESSAGE_THREE_HUMANOIDS_6` | The barbarian points to the end of the | 「Ariel.. Ookla.. こっちだ！」と叫ぶ。 | バーバリアンは廊下の奥を指さし、 |
| `GAME_MESSAGE_THREE_HUMANOIDS_7` | corridor   and yells  'Ariel.. Ookla.. | そして三人は走り去ってしまう！      | 「Ariel.. Ookla..」と叫ぶ。 |
| `GAME_MESSAGE_GETTING_WARM_3_1` |       Its almost too hot to bear!      |       ほとんど耐えられない暑さだ！      |        ほとんど耐えられない暑さだ！      |
| `CONFIRM_CHANGE_CLASS` | Are you sure you want to change the Class of this Character? This cannot be undone! | このキャラクターの職業を変更しますか？これは取り消せません。 | このキャラクターの職業を変更しますか？ 変更は取り消せません！ |
| `LEGATE_TITLE` | Choose Character to Legate | キャラクターを継承 | 継承するキャラクターを選ぶ |
| `CHANGE_CLASS_TITLE` | Change Class | 名前を変更 | 職業を変更 |

## 新規 156 キー

| キー | 英語 | 訳案 |
|---|---|---|
| `SPLASH_DETAILS` | Additional Thanks: Toriotto/Xelu (Graphics), Eric Matyas (Music), UI Engine (Dear Imgui/Omar Cornut) | 追加謝辞: Toriotto/Xelu（グラフィック）、Eric Matyas（音楽）、UI エンジン（Dear ImGui / Omar Cornut） |
| `DIALOG_RITE` | Are you sure? Legating cannot be undone! | よろしいですか？ 継承は取り消せません！ |
| `DIALOG_DELETE` | Are you sure? Deleting cannot be undone! | よろしいですか？ 削除は取り消せません！ |
| `AUTOMAP_TITLE` | Map | マップ |
| `AUTOMAP_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `REMOVE_CHARACTER_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `INN_SUBTITLE` | Who will stay? | 誰が泊まりますか？ |
| `INN_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `TEMPLE_SUBTITLE_1` | Welcome to the Temple of Cant | カントの寺院へようこそ |
| `TEMPLE_SUBTITLE_2` | Who needs our help? | 誰を治療しますか？ |
| `PAY_TITLE` | Temple | 寺院 |
| `PAY_SUBTITLE` | Who will tithe? | 誰が寄付しますか？ |
| `TEMPLE_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `SHOP_SUBTITLE` | Who will enter? | 誰が入りますか？ |
| `SHOP_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `OPT_CGA_GRAPHICS` | CGA Graphics | CGA グラフィック |
| `TRAINING_GROUNDS_ROSTER` | Roster of Characters | キャラクター名簿 |
| `DELETE_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `DELETE_TITLE` | Delete Character | キャラクターを削除 |
| `NOTICE_UNCURSED` | Done! | 解呪しました！ |
| `POPUP_PIT` | A Pit! | 落とし穴だ！ |
| `MODAL_EQUIP` | Equip which item? | どのアイテムを装備しますか？ |
| `MODAL_REMOVE` | Remove which item? | どのアイテムを外しますか？ |
| `MODAL_CHEST` | Who will do this? | 誰が行いますか？ |
| `MODAL_SPELL` | Cast which spell? | どの呪文を唱えますか？ |
| `EQUIP_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `REMOVE_ITEM_RETURN` | &lt;Return | &lt;戻る&gt; |
| `SPELL_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `CHEST_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `GAME_MESSAGE_MAN_TELEPORT_CASTLE_45` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_NEED_BEAR_STATUE_1` | Nobody in the party can break the door | パーティの誰にも扉を破ることは |
| `GAME_MESSAGE_NEED_BEAR_STATUE_2` | down.                                  | できない。 |
| `GAME_MESSAGE_NEED_FROG_STATUE_1` | Nobody in the party can break the door | パーティの誰にも扉を破ることは |
| `GAME_MESSAGE_NEED_FROG_STATUE_2` | down.                                  | できない。 |
| `GAME_MESSAGE_NEED_GOLD_KEY_1` | Nobody in the party can break the door | パーティの誰にも扉を破ることは |
| `GAME_MESSAGE_NEED_GOLD_KEY_2` | down.                                  | できない。 |
| `GAME_MESSAGE_LARGE_DESK_4_5` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_LARGE_DESK_4_6` | As you entered the room, a panel on the | 部屋に入ると、扉のパネルが |
| `GAME_MESSAGE_LARGE_DESK_6_5` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_TREBOR_VOICE_1_5` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_TREBOR_VOICE_2_7_5` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_WERDNA_BOAST_1_35` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_WERDNA_BOAST_2_3` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_WERDNA_SIGN_1_15` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_WERDNA_SIGN_1_45` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_WERDNA_SIGN_2_15` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_WERDNA_SIGN_2_45` | （空白 38 文字） | （空白 38 文字） |
| `GAME_MESSAGE_THREE_HUMANOIDS_8` | This Way!!' And they all run off!      | 「こっちだ！」三人は走り去った！ |
| `C_ACTION_REMOVE` | Remove | 外す |
| `STORE_TITLE` | Store | 交易所 |
| `BUY_TITLE` | Buy | 買う |
| `BUY_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `BUY_GOLD_P` | You have  | 所持金は  |
| `BUY_GOLD_S` |  G.P. |  GP |
| `BUY_WELCOME_P` | Welcome,  | ようこそ、 |
| `BUY_WELCOME_S` | .  Buy which item? | 。どのアイテムを買いますか？ |
| `SELL_TITLE` | Sell | 売る |
| `SELL_GOLD_P` | You have  | 所持金は  |
| `SELL_GOLD_S` |  G.P. |  GP |
| `SELL_WELCOME_P` | Welcome,  | ようこそ、 |
| `SELL_WELCOME_S` | .  Sell which item? | 。どのアイテムを売りますか？ |
| `IDENTIFY_TITLE` | Identify | 鑑定 |
| `IDENTIFY_GOLD_P` | You have  | 所持金は  |
| `IDENTIFY_GOLD_S` |  G.P. |  GP |
| `IDENTIFY_WELCOME_P` | Welcome,  | ようこそ、 |
| `IDENTIFY_WELCOME_S` | .  Identify which item? | 。どのアイテムを鑑定しますか？ |
| `SHOP_IDENTIFY_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `UNCURSE_TITLE` | Identify | 解呪 |
| `UNCURSE_GOLD_P` | You have  | 所持金は  |
| `UNCURSE_GOLD_S` |  G.P. |  GP |
| `UNCURSE_WELCOME_P` | Welcome,  | ようこそ、 |
| `UNCURSE_WELCOME_S` | .  Uncurse which item? | 。どのアイテムを解呪しますか？ |
| `SHOP_UNCURSE_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `STORE_WELCOME_P` | Welcome,  | ようこそ、 |
| `STORE_WELCOME_S` | . | 。 |
| `STORE_GOLD_P` | You have  | 所持金は  |
| `STORE_GOLD_S` |  G.P. |  GP |
| `NOTICE_RENAMED_OK` | Character has been renamed! | キャラクターの名前を変更しました！ |
| `NOTICE_RECLASSED_OK` | Character has changed class! | キャラクターの職業を変更しました！ |
| `CONFIRM_LEGATE` | Are you sure you wish to proceed? This is permanent! | 続行しますか？ この操作は取り消せません！ |
| `GRAVEYARD_RETURN` | Leave the CemeterY | 墓地を出る |
| `STORE_UNCURSE` | Uncurse | 解呪 |
| `STORE_IDENTIFY` | Identify | 鑑定 |
| `STORE_BUY` | Buy | 買う |
| `STORE_SELL` | Sell | 売る |
| `STORE_POOL` | Pool Gold | 所持金をまとめる |
| `STORE_RETURN` | Leave | 出る |
| `SELL_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `UNCURSE_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `NOTICE_CURSED` | * Cursed * | * 呪われている * |
| `NOTICE_FAILED` | Failed! | 失敗！ |
| `NOTICE_SUCCESS` | Success! | 成功！ |
| `NOTICE_OOPS` | Oops! | しまった！ |
| `EDIT_TITLE` | Edit Character | キャラクターを編集 |
| `EDIT_CHANGE_NAME` | Change Name | 名前を変更 |
| `EDIT_CHANGE_CLASS` | Change Class | 職業を変更 |
| `EDIT_LEGATE_CHARACTER` | Legate Character | キャラクターを継承 |
| `EDIT_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `RITE_TITLE` | Rite of Passage | 継承の儀式 |
| `RITE_PROGRESS` | Rite in Progress... | 儀式の進行中… |
| `RITE_STAGE_1` | The Rite of Passage Ceremony | 継承の儀式が |
| `RITE_STAGE_2` | Now begins... | 今、始まる… |
| `RITE_STAGE_3` | The Temple priests link up this | 寺院の司祭たちが、この祖先の魂を |
| `RITE_STAGE_4` | ancestral spirit with its descendant... | 子孫と結びつける… |
| `RITE_STAGE_5` | The rite is completed! | 儀式は完了した！ |
| `SELECT_TITLE` | Choose Character to Rename | 名前を変更するキャラクターを選ぶ |
| `SELECT_RETURN` | &lt;Returh&gt; | &lt;戻る&gt; |
| `RETRAIN_TITLE` | Choose Character to Retrain | 再訓練するキャラクターを選ぶ |
| `RETRAIN_RETURN` | &lt;Returh&gt; | &lt;戻る&gt; |
| `LEGATE_RETURN` | &lt;Returh&gt; | &lt;戻る&gt; |
| `RENAME_PROMPT` | Enter New Name: | 新しい名前を入力: |
| `CHANGE_CLASS_RETURN` | &lt;Return&gt; | &lt;戻る&gt; |
| `ELEVATOR_BOTTOM` | There are buttons here marked A to F. | ここには A から F のボタンがある。 |
| `ELEVATOR_TOP` | There are buttons here marked A to D. | ここには A から D のボタンがある。 |
| `DIALOG_SEARCH` | Will you search? | 探しますか？ |
| `POP_UP_FOUND_AN_ITEM` | has found an item! | がアイテムを見つけた！ |
| `POP_UP_PARTY_FOUND_AN_ITEM` | The party has found an item! | パーティがアイテムを見つけた！ |
| `POP_UP_ELEVATOR` | One Moment! | 少々お待ちください！ |
| `VICTORY_1` | * * CONGRATULATIONS * * | * * おめでとう！ * * |
| `VICTORY_2` | You have recovered the Amulet | アミュレットを取り戻し、 |
| `VICTORY_3` | and passed the Overlord's test! | 君主の試練を乗り越えた！ |
| `VICTORY_4` | As a reward, the Mad Overlord | 褒美として、狂王は |
| `VICTORY_5` | has granted you 50000 GP and EP! | 50,000 GP と EP を与えた！ |
| `VICTORY_6` | He has also inducted you into his elite guard | さらにあなたを精鋭親衛隊に迎え入れた。 |
| `VICTORY_7` | Wear the Chevron of Rank (&gt;) with Honour | 階級章（&gt;）を誇りを持って身につけよ。 |
| `VICTORY_8` | THE END. | 完。 |
| `VICTORY_9` | (Or is it?) | （本当に？） |
| `VICTORY_LEAVE` | &lt;Return&gt; | &lt;戻る&gt; |
| `CHEST_MENU_TITLE` | A Chest! You may: | 宝箱だ！ どうしますか？ |
| `CHEST_MENU_OPEN` | Open | 開ける |
| `CHEST_MENU_INPSECT` | Inspect | 調べる |
| `CHEST_MENU_CALFO` | CALFO | CALFO |
| `CHEST_MENU_DISARM` | Disarm | 罠を解除 |
| `CHEST_MENU_LEAVE` | &lt;Leave&gt; | &lt;離れる&gt; |
| `CHEST_NO_TRAP` | The Chest is not trapped! | 宝箱に罠はない！ |
| `CHEST_POISON_NEEDLE` | Poison Needle | 毒針 |
| `CHEST_GAS_BOMB` | Gas Bomb | ガス爆弾 |
| `CHEST_CROSSBOW_BOLT` | Crossbow Bolt | クロスボウの矢 |
| `CHEST_EXPLODING_BOX` | Exploding Box | 爆発する箱 |
| `CHEST_SPLINTERS` | Splinters | 木片 |
| `CHEST_BLADES` | Blades | 刃 |
| `CHEST_STUNNER` | Stunner | スタンナー |
| `CHEST_TELEPORTER` | Teleporter | テレポーター |
| `CHEST_MAGE_BLASTER` | Mage Blaster | メイジブラスター |
| `CHEST_PRIEST_BLASTER` | Priest Blaster | プリーストブラスター |
| `CHEST_ALARM` | Alarm | 警報 |
| `CHEST_TRAP_TITLE` | Select Trap: | 罠を選ぶ: |
| `CHEST_INSPECT_TITLE` | Who will inspect? | 誰が調べますか？ |
| `CHEST_OPEN_TITLE` | Who will open? | 誰が開けますか？ |
| `CHEST_CALFO_TITLE` | Who will cast a Spell? | 誰が呪文を唱えますか？ |
| `CHEST_DISARM_TITLE` | Who will disarm? | 誰が罠を解除しますか？ |
| `CHEST_TRAP_LEAVE` | &lt;Leave&gt; | &lt;離れる&gt; |
| `CHEST_OOPS` | Oops!  | しまった！  |
| `CHEST_DISARMED` | You disarmed it! | 罠を解除した！ |
| `CHEST_DISARM_FAILED` | Disarm failed! | 罠の解除に失敗！ |
| `CHEST_ALREADY_INSPECTED` |  has already inspected! |  はすでに調べている！ |
