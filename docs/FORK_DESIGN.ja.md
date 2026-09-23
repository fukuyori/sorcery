# フォーク（Windows / 日本語版）の対応方針と設計

最終更新: 2026-09-23

このフォーク <https://github.com/fukuyori/sorcery> は
<https://github.com/davemoore22/sorcery>（以下 upstream）を元に、Windows で動作する
日本語版を提供する。本書は upstream の Alpha.1 での履歴書き換えを受けて決めた
対応方針と、日本語化・フォント対応を upstream の新しい土台へ移植する設計をまとめる。

---

## 1. 背景

### 1.1 upstream 側で起きたこと（2026-09）

- 開発ブランチ `sdl/imgui` が削除され、全履歴が 1 コミット（`7ad8b6ed` "Alpha 1
  Squashed Commit"）に潰された上で `master` が作り直された。タグ `Alpha.1` が付いている。
- 開発は `Alpha_2`（コンテキストヘルプ、音量調整、テクスチャ解放）と
  `Alpha_2_Spellcasting`（呪文詠唱、Game ライブラリへの再構成）で続いている。
- ソース構成が `src/core/controller/`、`src/display/ui/` などに再編された。
- フォントの置き場が `dat/` 直下から `dat/fonts/` に移った。
- ビルドは CMake の FetchContent で ImGui、cereal、SimpleIni、stb などを取得する
  方式になり、`ext/` は不要になった。
- C++26 のリフレクション（`src/types/meta.cpp`、列挙型と文字列の相互変換）を使う
  ため、GCC 16.1 以上が必須。Windows の公式ビルド環境は MSYS2 UCRT64 で、
  MSVC は未検証と明記されている。

### 1.2 フォーク側の状況

- 旧主ブランチ `sdl/imgui` は旧 upstream（`94506135`）の上に 9 コミット +
  マージ + 1 コミットを積んでいた。内容は Windows 対応（MSVC + vcpkg）、日本語
  ローカライズ、フォント対応、宝箱の骨組み、ドキュメント。
- 新しい upstream とは共通の祖先が無く、`git merge` は unrelated histories として
  拒否される。両者が触っているファイルは 46 件で、フォークの中核ファイルはほぼ
  すべて含まれる。

---

## 2. 対応方針

### 2.1 決定事項

| 項目 | 決定 | 理由 |
|---|---|---|
| 取り込み方法 | upstream の新ブランチを土台に、フォーク独自の変更を機能単位で移植する | 強制マージは 46 件以上の衝突解決になり、結果の見通しが悪い |
| 土台ブランチ | `upstream/master`（Alpha.1） | 最も安定。`Alpha_2` 系は変化が激しく再移植が増える |
| ビルド環境 | MSYS2 UCRT64 + GCC 16.2（upstream と同一） | 下記 2.2 |
| 旧 Windows 対応 | 廃止（vcpkg.json、PowerShell スクリプト、MSVC 互換ヘッダ、`ext/`） | upstream が Windows を公式サポートしたため重複 |

### 2.2 ビルド環境の比較（MSYS2 + GCC 16 と MSVC 維持）

MSVC を維持する案は、`meta.cpp` を magic_enum で置き換え、CMake に MSVC 用の分岐
（GCC 専用の警告フラグ約 25 個、`stdc++fs` のリンク、pkg-config による FFmpeg
検出）を追加すれば成立する見込みだった。しかし

- upstream は履歴の squash とディレクトリ再編を行っており、独自の CMake 分岐は
  追従のたびに壊れる可能性が高い。
- upstream の作者は MSVC を試していないため、GCC 拡張や C++26 機能の追加が即
  エラーになりうる。
- フォークの価値は日本語化とフォント対応にあり、ビルド環境の保守に工数を割く
  意味が薄い。

以上から MSYS2 + GCC 16 を採用した。Visual Studio のデバッガは使えなくなり、
デバッグは GDB（VS Code + CMake Tools）になる。

### 2.3 検証結果（2026-09-23）

- MSYS2 を `C:\msys64` に導入し、UCRT64 に gcc 16.2.0、cmake 4.4.3、ninja 1.13.2、
  pkgconf、SDL2、freetype、glm、ffmpeg、glew、jsoncpp を導入。
- `upstream/master` のソースを無変更で `cmake -S . -B build` と
  `cmake --build build --parallel` にかけ、警告 0、エラー 0 でビルド成功。
- `build/dist/sorcery.exe` の起動と英語タイトル画面の表示を確認。

### 2.4 upstream の既知の問題（フォークで修正済み）

| Issue | 内容 | 修正 |
|---|---|---|
| #1 | `sav/characters` と `sav/states` が Git に追跡されず、新規クローンでは起動時に必須ディレクトリ不在で終了する | PR #4: ポストビルドで `make_directory` |
| #2 | `Win32RuntimeDependencies.cmake` の除外正規表現が `C:\WINDOWS\system32` の大文字に一致せず、システム DLL 246 個が `build/dist` にコピーされる | PR #3: 大文字小文字非依存の文字クラスに変更し、`CMP0207` を NEW に設定 |

どちらも upstream の `master`、`Alpha_2`、`Alpha_2_Spellcasting` で未修正。
upstream への報告は別途判断する。

---

## 3. ブランチ構成と運用

### 3.1 構成

| ブランチ | 役割 | コミット |
|---|---|---|
| `master`（既定） | フォークの主ブランチ。`upstream-master` + フォークの変更 | PR 経由のみ |
| `upstream-master` | `upstream/master` のミラー | 禁止。fast-forward のみ |
| `fix/<name>` | upstream にも送れる修正。**`upstream-master` から分岐** | PR → `master` |
| `feature/<name>` | フォーク固有の機能。`master` から分岐 | PR → `master` |
| `sdl/imgui` | 旧主ブランチ。アーカイブ | 禁止 |
| `upstream-sdl-imgui` | 旧 upstream 追跡。削除候補 | 禁止 |

`fix/` を `upstream-master` から分岐させるのは、フォーク固有のコミットを含まない
状態で upstream に PR を出せるようにするため。

### 3.2 upstream 追従の手順

```sh
git fetch upstream
git checkout upstream-master && git merge --ff-only upstream/master
git checkout master && git merge upstream-master
```

`master` へはマージコミットで取り込む。フォークのコミットをリベースし続けると
`origin/master` を毎回強制プッシュすることになるため。

### 3.3 upstream が再度 squash した場合

Alpha.2 以降で再び履歴が書き換えられた場合は、本書の手順を繰り返す。

1. `upstream-master` を新しい `upstream/master` に付け替える（`git branch -f`）。
2. 新しい `master` を `upstream-master` から切り直す。旧 `master` は
   `archive/master-alpha1` などの名前で残す。
3. 5 章の機能単位で移植をやり直す。CHANGELOG.md の一覧を移植チェックリストに使う。

再移植を軽くするため、フォークの変更は機能ごとに小さな PR に分け、
CHANGELOG.md に必ず記録する。

---

## 4. ビルド手順（Windows）

upstream の `doc/COMPILE.md` に従う。要点のみ記す。

```sh
# MSYS2 UCRT64 シェルで
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake \
          mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-pkgconf git
pacman -S mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-freetype \
          mingw-w64-ucrt-x86_64-glm mingw-w64-ucrt-x86_64-ffmpeg \
          mingw-w64-ucrt-x86_64-glew mingw-w64-ucrt-x86_64-jsoncpp
cd /d/home/source/cpp/sorcery
cmake -S . -B build -G Ninja
cmake --build build --parallel
./build/dist/sorcery.exe
```

補足

- MSYS2 の git-lfs は `mingw-w64-ucrt-x86_64-git-lfs`。ホストの Git for Windows に
  git-lfs があれば不要。
- `CMAKE_BUILD_TYPE` は CMakeLists.txt で Debug 固定。exe は約 170 MB になる。
  配布用の Release ビルドは未整備（7 章）。
- ポストビルドでリポジトリの `cfg/`、`dat/` などが `build/dist/` に上書きコピー
  される。`build/dist/cfg/config.ini` を直接編集した場合はビルドのたびに戻るため、
  リポジトリ側の `cfg/config.ini` を編集するかバックアップを取る。

---

## 5. 移植設計

旧 `sdl/imgui` の変更を、upstream の新構成に載せ直す。各項目を独立した
`feature/` ブランチと PR にする。

### 5.1 日本語文字列（`feature/ja-strings`）

**現状の upstream**

- `StringStore`（`inc/resources/stringstore.hpp`）は単一ファイル
  `dat/strings.json` を読むだけ。`get(key)` は未登録キーに `KEY_NOT_FOUND` を返す。
- `System` が `StringStore` を生成した後に `Config` を読む順序になっている
  （`src/core/system.cpp`）。言語設定はまだ存在しない。

**設計**

- `StringStore::load_overlay(path)` を追加する。既存の `_load()` を
  `_load_file(path, clear_existing)` に一般化し、オーバーレイでは既存エントリを
  上書きするだけで削除しない。これにより未翻訳キーは英語にフォールバックする。
  オーバーレイの読み込みに失敗した場合は英語データを保持し、失敗を報告する。
  `reload()` でも英語データを読み直した後に選択中のオーバーレイを再適用する。
- `cfg/config.ini` に `[Localization] language = en` を追加する。
- `System` で `Config` 読み込み後に言語を判定し、`ja` または `ja-jp`（大文字小文字
  無視）なら `dat/strings.ja.json` をオーバーレイする。`StringStore` の生成が
  `Config` より前なので、生成順を入れ替えるか、オーバーレイ呼び出しを `Config`
  の後に置く。
- `inc/resources/define.hpp` に `STRINGS_JA_FILE{"strings.ja.json"}` を追加する。
- `FileStore` に `strings.ja.json` を任意ファイルとして登録する（必須にすると
  英語のみの配布で起動しなくなる）。日本語指定時にファイルが無い場合も英語で
  起動できることを確認する。

**翻訳データ**

旧 `strings.ja.json`（948 キー）と upstream の `strings.json`（1065 キー）の突合:

| 区分 | 件数 | 対応 |
|---|---|---|
| 両方に存在し原文不変 | 883 | そのまま流用 |
| 両方に存在し英語原文が変更 | 26 | 訳を見直す |
| upstream にのみ存在 | 156 | 新規翻訳 |
| 旧フォークにのみ存在 | 39 | 廃止（削除） |

- クラス名、種族名、属性名、呪文名はローマ字のまま（旧フォークの慣例を踏襲）。
- `strings.json` と `strings.ja.json` のキー集合が一致することを検査する簡単な
  スクリプトを `scripts/` に置く（Python）。
- 原文変更 26 キーと新規 156 キーは訳案を作成し、
  `docs/ja-translation-review.md` に旧訳・原文・訳案を並べてレビューする。
  日本語画面での見え方はフォント対応後に確認する。

### 5.2 UI ラベルの文字列キー化（`feature/ja-strings` に含める）

- キャラクター詳細・サマリーの能力値ラベルは旧フォークで `CHARACTER_*` キー経由に
  変更していた。upstream の `src/display/ui/ui.cpp` で同等箇所を探して当て直す。
  該当箇所はソース再編で移動しているため、旧差分（`git diff 94506135 sdl/imgui --
  src/core/ui.cpp`）を参照しながら手作業で移植する。

### 5.3 メニューの中央揃え（`feature/ja-menu-align`）

**現状の upstream**

- `MenuBuilder::_load_fixed_menu`（`src/core/controller/menubuilder.cpp`）ほか
  数箇所で `std::format("{:^{}}", text, width)` により空白パディングで中央揃えして
  いる。文字幅による空白パディングは ImGui のフォントで測ったピクセル幅と
  一致せず、日本語では目視でずれる。
- `src/display/ui/ui.cpp` の Selectable 描画は、横並び（`across`）のときだけ
  `ImGuiStyleVar_SelectableTextAlign` を 0.5 にしている。

**設計**

- 中央揃え用の `std::format` の幅指定を外し、文字列をそのまま items に入れる。
  `##` 以降の ImGui 識別子は保持する。名前・所持金など複数列を空白で整列して
  いる項目は対象外とし、別途表示を確認する。
- 縦並びでも中央揃えが必要なメニューに限り、`SelectableTextAlign` を
  `{0.5, 0.5}` にして ImGui のピクセル幅計測で中央揃えする。`across` だけを
  条件とする現行処理に、行の種類による判定を加える。
- 固定選択肢は `MenuBuilder` の `data` に対応する要素を持たないため、その行を
  中央揃えする。キャラクター名・所持金など `data` がある行は従来の列配置を保つ。
- 確認ダイアログは本文幅だけでなくボタン列の幅からも最小幅を決める。
  本文が折り返される場合は、その高さに合わせてボタン位置とダイアログ高を広げる。
- タイトル画面の歓迎文・メニューと終了確認ダイアログのみ、レイアウトの
  `font_scale` を 1.2 にして文字を大きくする。メニューは行間を 0 にして
  7 項目を既存の枠内に収める。他画面の文字サイズは変更しない。
- 英語表示でも文字や選択範囲が枠からはみ出さないことをスクリーンショットで確認する。

### 5.4 フォント（`feature/ja-fonts`）

**現状の upstream**

- `FontStore` は `dat/fonts/` の `.ttf` のみ走査し、サイズ 0 で `AddFontFromFileTTF`
  する（ImGui 1.92 の動的サイズ方式）。描画時に `UiMetrics::font_sz()` で
  サイズを与える。
- `config.ini` の `[Font]` は `monospace` / `proportional` / `text` の名前のみ。
  `size` は無い。
- `get_font_by_name` は大文字小文字を区別する。
- グリフ範囲は指定していない（ImGui 1.92 の動的方式では読み込み時の範囲指定が
  不要な場合がある。要確認: `GlyphRanges` を設定せずに日本語が描画できるか）。

**設計**

- `.ttc` を走査対象に加える。
- `get_font_by_name` を大文字小文字無視にする（FreeType が `M+ 2m bold` のように
  小文字のスタイル名を返すフォントがあるため）。旧フォークの修正を移植。
- 日本語グリフは現行の動的方式で表示できることを M+ フォントと日本語タイトル画面で
  確認済み。ImGui 1.92 ではグリフ範囲指定は旧方式向けのため追加しない。
- `[Font]` に任意キー `rasterizer_multiply`（疑似ボールド）と
  `glyph_extra_advance_x`（字間）を移植する。
- Noto Sans JP のウェイト 500 から生成した `Sorcery JP Sans Medium` を
  `dat/fonts/` に同梱する。生成方法とライセンスは `docs/ja-font.md` に記録する。
  このフォントで日本語タイトル画面と終了確認を表示できることを確認した。
- M+ FONTS（SIL OFL 1.1）は任意の代替フォントとし、利用者が配置する。
  ライセンス全文 `dat/fonts/OFL-mplus.txt` を同梱する。
- `cfg/config.ini` に日本語用設定をコメントアウトで記載する:

```ini
[Font]
; 日本語表示用 (同梱フォント):
; monospace = Sorcery JP Sans Medium
; proportional = Sorcery JP Sans Medium
; text = Sorcery JP Sans Medium
; M+ フォントを別途配置する場合:
; monospace = M+ 2m Bold
; proportional = M+ 2p Bold
; text = M+ 1p Regular
monospace = Wizardry 5 DOS Regular
proportional = Marcellus Regular
text = ProggyVector Regular
; 省略時は ImGui の既定値 (1.0 / 0.0):
; rasterizer_multiply = 1.0
; glyph_extra_advance_x = 0.0

[Localization]
; 日本語表示用:
; language = ja
language = en
```

### 5.5 移植しないもの

| 旧フォークの変更 | 理由 |
|---|---|
| vcpkg.json、旧 scripts/build-windows.ps1、scripts/restore-ext.ps1 | MSYS2 方式に統一。配布用の新しいスクリプトは 7 章で別途整備する |
| MSVC 互換ヘッダ（macro.hpp、parse.hpp、json.hpp の変更） | GCC でビルドするため不要 |
| `%APPDATA%\Sorcery\save.db3` への保存先変更 | upstream はセーブ方式を `sav/` 配下の JSON に変更済み |
| `Chest` 型の骨組み | upstream に `inc/engine/chest.hpp` が実装済み |
| `Game::give_item_to_party` | upstream の再構成後に必要性を再評価。Alpha_2_Spellcasting の Game ライブラリを確認してから判断 |
| doc/WINDOWS.md、doc/LINUX.md | upstream の doc/COMPILE.md が両 OS を網羅 |
| doc/WIZARDRY_CODE_REFERENCE_PLAN.md | 内容を確認し、有用なら `docs/` に移す（別 PR） |

### 5.6 ドキュメント（`feature/ja-docs`）

- README.md: 冒頭にフォークの説明（英語）、「Changes from upstream」、
  「Japanese display setup」、区切り線の後に upstream README の全文転載。
- README.ja.md: 同内容の日本語版（upstream README は転載せず参照）。
- CHANGELOG.md: Keep a Changelog 形式。Alpha.1 土台への移行を記録し、5.5 の
  「移植しないもの」も Removed として明記する。
- 本書（docs/FORK_DESIGN.ja.md）。

**文書の置き場所**

- フォーク独自に新しく作る文書は `docs/` に置く（本書、バージョン更新チェック
  リスト、今後の設計・運用文書など）。README.md、README.ja.md、CHANGELOG.md は
  慣例どおりリポジトリ直下に置く。
- `doc/` は upstream の文書置き場で、ビルドのポストビルド処理で `build/dist/doc/`
  にコピーされ、`inc/resources/define.hpp` の `COMPILE_FILE` / `LICENSE_FILE` から
  ゲーム内で参照される。フォーク側では変更せず、既存文書の移動も行わない。
  移動が必要になった場合は上記の参照先を確認したうえで別途扱う。

---

## 6. 作業順序

| 順 | ブランチ | 内容 | 依存 |
|---|---|---|---|
| 1 | `feature/ja-strings` | オーバーレイ機構、config、`strings.ja.json`（883 + 26 見直し + 156 新規） | なし |
| 2 | `feature/ja-menu-align` | 中央揃えの `SelectableTextAlign` 化 | 1（日本語で検証するため） |
| 3 | `feature/ja-fonts` | `.ttc`、大文字小文字無視、日本語フォント同梱、Windows GUI 起動 | 1〜2 |
| 4 | `feature/ja-docs` | README、README.ja、CHANGELOG | 1〜3 |

この順序で移植した。英語表示と日本語表示をそれぞれ確認する。

---

## 7. 未決事項

- 配布用 Release ビルドの整備。CMakeLists.txt が Debug 固定のため、フォーク側で
  `CMAKE_BUILD_TYPE` を外すか、upstream に提案するか。配布時には Release ビルド用と
  Inno Setup インストーラー作成用の 2 種類のスクリプトを用意する。後者の `-Sign`
  オプションは環境変数 `CODESIGN_CERT` を使い、実行ファイル、インストーラー、
  アンインストーラーに電子署名する。
- バージョン変更時に更新するファイルの一覧を
  `docs/version-update-checklist.md` に用意する。
- Issue #1、#2 の upstream への報告。
- `Alpha_2` 系への追従時期。`Alpha_2_Spellcasting` は Game ライブラリの再構成を
  含むため、移植完了後に差分を確認して判断する。
