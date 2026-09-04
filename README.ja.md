# Sorcery（Windows / 日本語版）

[English](README.md)

このリポジトリは <https://github.com/davemoore22/sorcery> のフォークで、Windows で動作する日本語版です。
以下にフォーク元からの変更点と日本語表示の設定方法を記載します。
フォーク元の README（英語）は [README.md](README.md) の後半にそのまま転載しています。

## フォーク元からの変更点

### Windows ビルド・実行対応

- `vcpkg.json` を追加し、依存ライブラリ（SDL2、SDL2_image、GLEW、jsoncpp、FreeType、
  FFmpeg、sqlite3 など）を vcpkg で解決できるようにした。
- `CMakeLists.txt` の `find_package` を vcpkg の CONFIG モードに対応させ、
  `SDL2::SDL2` / `GLEW::GLEW` / `JsonCpp::JsonCpp` などのインポートターゲットを
  環境に応じて選択するようにした。
- `scripts/restore-ext.ps1`（`ext/` 配下のサードパーティソースの取得）と
  `scripts/build-windows.ps1`（Visual Studio + CMake でのビルド）を追加した。
- MSVC でコンパイルできるよう、`inc/common/macro.hpp` `parse.hpp` `json.hpp` などの
  共通ヘッダを整理した。
- データ・設定ファイルのパスを実行ファイルの場所を基準に解決するようにし、
  カレントディレクトリに依存せず起動できるようにした。
- セーブデータを `%APPDATA%\Sorcery\save.db3` に保存し、ファイルが無い場合は
  必要なテーブルを起動時に自動作成するようにした。リポジトリ同梱の `dat/save.db3` は
  削除し、`.gitignore` に追加した。
- Windows / Linux のビルド手順を `doc/WINDOWS.md` `doc/LINUX.md` に記載した。

### 日本語ローカライズ

- `dat/strings.ja.json` を追加し、`dat/strings.json` の全キー（948 件）を翻訳した。
  クラス名・種族名・属性名・呪文名はプロジェクトの慣例に従いローマ字のまま。
- `StringStore` にオーバーレイ読み込み（`load_overlay`）を追加した。
  `[Localization] language = ja` のとき `strings.ja.json` を英語の上に重ねて読み込み、
  未翻訳キーは英語にフォールバックする。
- キャラクター詳細・サマリーパネルの能力値ラベルを文字列キー（`CHARACTER_*`）経由に
  変更し、言語設定に追従するようにした。
- メニュー項目の中央揃えを `std::format` の幅指定から ImGui の
  `SelectableTextAlign` によるピクセル幅基準に変更し、全角文字でずれる問題を修正した。

### フォント

- フォントの検索ディレクトリを実行ファイル基準の `dat/` に修正した。
- `.ttf` に加えて `.ttc`（TrueType Collection）も読み込むようにした。
- `[Font]` に `rasterizer_multiply`（疑似ボールド）と `glyph_extra_advance_x`
  （字間の拡張）のオプションを追加した。
- ImGui の日本語グリフ範囲を有効にした。
- `config.ini` のフォント名の照合を大文字小文字を区別しないようにした
  （FreeType が返すスタイル名が `bold` / `regular` のように小文字になるフォントがあるため）。
- 日本語フォントとして M+ FONTS（M+ FONTS PROJECT / 森下浩司、SIL Open Font License 1.1、
  <https://github.com/rayshan/mplus-fonts>）を利用できるようにし、ライセンス全文を
  `dat/OFL-mplus.txt` に同梱した。

### ゲーム内容

- パーティのインベントリが満杯の場合を考慮したアイテム付与ヘルパー
  `Game::give_item_to_party` を追加した。
- 宝箱（`Chest` 型）の骨組みを追加した。罠種別・結果の列挙型と、Calfo / 調査・解除・
  開封のスタブを含む。

### ドキュメント

- `doc/WIZARDRY_CODE_REFERENCE_PLAN.md` を追加した。Wizardry.Code を参照した実装計画と、
  呪文挙動（Latumapic、Manifo、Loktofeit、Haman、Mahaman、Montino、Mamorlis、Mabadi）の
  監査チェックリストを含む。

## 日本語表示の設定

日本語表示には、日本語グリフを含むフォントと `cfg/config.ini` の設定変更が必要です。

### 1. フォントの配置

M+ FONTS（SIL OFL 1.1、ライセンス全文は `dat/OFL-mplus.txt`）を
<https://github.com/rayshan/mplus-fonts/tree/master/fonts> から取得し、次の
3 ファイルを `dat/` に置きます。

- `mplus-1p-regular.ttf`
- `mplus-2m-bold.ttf`
- `mplus-2p-bold.ttf`

### 2. 設定ファイルの書き換え

`cfg/config.ini` には日本語用の設定がコメントアウトして記載してあります。
コメント行の `;` を外し、元の英語用の行を削除（またはコメントアウト）します。

書き換え前:

```ini
[Font]
size = 24
; 日本語表示用 (dat/ に mplus-*.ttf を配置した場合):
; monospace = M+ 2m Bold
; proportional = M+ 2p Bold
; text = M+ 1p Regular
monospace = Wizardry 5 DOS Regular
proportional = Marcellus Regular
text = ProggyVector Regular

[Localization]
; 日本語表示用:
; language = ja
language = en
```

書き換え後:

```ini
[Font]
size = 24
monospace = M+ 2m Bold
proportional = M+ 2p Bold
text = M+ 1p Regular

[Localization]
language = ja
```

フォント名は大文字小文字を区別しません。

### 3. 注意: ビルドで設定ファイルが上書きされる

ビルドのポストビルド処理で、リポジトリの `cfg/` と `dat/` がビルドディレクトリ
（例: `build-windows-ninja/cfg/`、`build-windows-ninja/dat/`）へコピーされます。
ビルドディレクトリ側の `config.ini` を直接編集していた場合は、ビルドのたびに
リポジトリ側の内容で上書きされるので、事前にバックアップを取るか、リポジトリ側の
`cfg/config.ini` を書き換えてください。
