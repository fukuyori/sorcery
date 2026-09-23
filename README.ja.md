# Sorcery Windows・日本語フォーク

このリポジトリは [Dave Moore 氏の Sorcery](https://github.com/davemoore22/sorcery)
の Alpha #1 を土台にしたフォークです。日本語の文字列とフォントを追加しています。
ゲームは開発中です。初期設定は英語で、日本語は設定ファイルから選択できます。

## upstream からの変更

- 日本語の文字列を英語データに重ねて読み込みます。日本語データの読み込みに
  失敗したときは、エラーを報告して英語で起動します。
- Noto Sans JP から生成した `Sorcery JP Sans Medium` を同梱しました。
  出典、ライセンス、再生成方法は[日本語フォントの文書](docs/ja-font.md)を参照してください。
- 日本語メニューの中央揃え、タイトル画面の文字サイズ、確認ダイアログの
  大きさを調整しました。
- Windows では起動時にターミナルを開かない GUI アプリとしてビルドします。
- 新規ビルドで必要な保存先ディレクトリの作成と、Windows の依存 DLL の
  コピー処理を修正しました。

移植の判断は[フォークの設計文書](docs/FORK_DESIGN.ja.md)、変更履歴は
[CHANGELOG.md](CHANGELOG.md)に記録しています。

## 日本語表示の設定

ビルド後、`build/dist/cfg/config.ini` を開き、次の値を指定します。

```ini
[Font]
monospace = Sorcery JP Sans Medium
proportional = Sorcery JP Sans Medium
text = Sorcery JP Sans Medium

[Localization]
language = ja
```

フォントと `dat/strings.ja.json` はビルド出力に含まれます。
`build/dist` を作業ディレクトリにして `sorcery.exe` を起動してください。
実行ファイルを再リンクすると、ポストビルド処理で `cfg/config.ini` が
`build/dist/cfg/config.ini` に再コピーされます。設定をビルド後も維持する
場合は、ソース側の `cfg/config.ini` を編集してください。

同梱フォントで日本語のタイトル画面と終了確認を目視確認しました。
その他の画面は今後確認します。

Windows のビルド環境と手順は [doc/COMPILE.md](doc/COMPILE.md) を参照してください。
このフォークは MSYS2 UCRT64 / GCC を使用します。

## upstream とライセンス

ゲームの概要やスクリーンショットは [README.md の upstream 部分](README.md#upstream-readme-unchanged)
を参照してください。プログラムのライセンスは [doc/LICENSE.md](doc/LICENSE.md)、
同梱フォントのライセンスは [dat/fonts/OFL-sorcery-jp-sans.txt](dat/fonts/OFL-sorcery-jp-sans.txt)
にあります。
