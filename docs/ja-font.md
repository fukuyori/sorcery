# 日本語フォント

`dat/fonts/sorcery-jp-sans-medium.ttf` は Noto Sans JP の可変フォントから
ウェイト 500 を取り出し、ファミリー名を `Sorcery JP Sans` に変更したものです。
日本語 UI の文字幅と高さを、従来の Wizardry 5 DOS フォントに近づけるために
生成しました。元フォントの名前は変更後のフォント名に使用していません。

## 出典とライセンス

- 元フォント: [Google Fonts / Noto Sans JP](https://github.com/google/fonts/tree/main/ofl/notosansjp)
- 元フォントの SHA-256: `5113756f8a3b5d01b2211025e267c50121e3b36f465b7bbaf3cdaf4c3430bfd0`
- ライセンス: SIL Open Font License 1.1。著作権表示と全文は
  [`dat/fonts/OFL-sorcery-jp-sans.txt`](../dat/fonts/OFL-sorcery-jp-sans.txt) に収録。
- 生成フォントの SHA-256: `407347fc527a37e48d3e6a9f212117b5723e58c8bd23db92aeb609b56f5f4925`

## 再生成

Python と `fonttools` を用意し、上記ハッシュと一致する
`NotoSansJP-VF.ttf` を取得してから実行します。

```powershell
python -m pip install fonttools
python scripts/generate-jp-font.py C:\path\to\NotoSansJP-VF.ttf
```

スクリプトは `dat/fonts/sorcery-jp-sans-medium.ttf` を生成します。
使用するフォント名は `Sorcery JP Sans Medium` です。日本語表示では
`cfg/config.ini` の `[Font]` の `monospace`、`proportional`、`text` に
この名前を指定します。同じファイルの `[Localization]` は `language = ja`
にします。

フォントファイルを `dat/fonts/` に追加しただけでは、既存の
`build/dist/dat/fonts/` には反映されません。配布用ビルドのファイルコピーを
実行するか、変更したフォントを配布先へコピーして確認します。
