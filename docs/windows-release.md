# Windows Release とインストーラー

Windows の配布用ビルドは MSYS2 UCRT64 / GCC と Inno Setup 6 を使用する。
実行ファイルは GUI サブシステムでリンクされ、起動時にターミナルを開かない。

## Release ビルド

PowerShell から実行する。

```powershell
.\scripts\build-release.ps1
```

スクリプトは `C:\msys64` の UCRT64 ツールを使い、`build-release/dist/` に
実行ファイル、依存 DLL、データを作成する。ビルド種別、必要な日本語データと
フォント、実行ファイルの GUI サブシステムを確認する。
MSYS2 の場所が異なる場合は `-Msys2Root`、並列数は `-Jobs` で指定できる。

## Inno Setup インストーラー

```powershell
.\scripts\build-installer.ps1
```

このスクリプトは Release ビルドを実行し、別のステージング領域から
`dist/windows/Sorcery-JA-<version>-Setup.exe` と `.sha256` を作成する。
すでに Release ビルドを確認済みの場合は `-SkipBuild` を指定できる。
Inno Setup 6 の `ISCC.exe` が標準の場所に無い場合は `-ISCCPath` で指定する。

インストーラーは英語と日本語の UI を持ち、管理者権限で全ユーザー向けに
インストールする。
プログラム本体は Program Files に置き、初回起動時に既定の設定と初期セーブを
各ユーザーの `%LOCALAPPDATA%\Sorcery-JA` にコピーする。その後の設定変更や
セーブはこのユーザー別の場所に保存する。アンインストール時もこのデータは
保持する。
ソースの `cfg/config.ini` は同梱の日本語フォントと `language = ja` を既定とし、
インストール版もポータブル版も設定変更なしで日本語表示する。
以前のインストーラーが作成した英語の既定設定（`installer/config.legacy-en.ini`
と同一のもの）が完全に未変更の場合に限り、次回起動時に日本語の既定設定へ
更新する。変更済みのユーザー設定は保持する。

## 署名

`CODESIGN_CERT` に PFX ファイルのパス、Windows 証明書ストアの
SHA-1 拇印、または証明書のサブジェクト名を設定し、`-Sign` を付けて実行する。
PFX にパスワードが必要な場合は `CODESIGN_CERT_PASSWORD` に設定する。
拇印は空白やコロンを含んでもよい。

```powershell
.\scripts\build-installer.ps1 -Sign
```

スクリプトは署名ツールを事前確認し、ステージングした
`sorcery.exe` を署名する。Inno Setup の `SignTool` と
`SignedUninstaller=yes` によりインストーラーとアンインストーラーを署名し、
ステージングした実行ファイルとインストーラーの署名を
`signtool verify /pa` で確認する。アンインストーラーは Inno Setup が
コンパイル中に署名してインストーラーに格納するため、インストール後に
展開された `unins???.exe` で署名を確認する。
証明書が使用できない場合は署名処理で停止する。
SHA-256 と RFC 3161 タイムスタンプを使用する。タイムスタンプ URL は
`-TimestampUrl` で変更できる。`-SignToolPath` で署名ツールの場所を指定できる。

署名されるのはステージング側の実行ファイルで、
`build-release/dist/sorcery.exe` は変更されない。署名付き成果物を
作成するには、利用できる秘密鍵付きの証明書が必要になる。`-Sign` の
既定出力先は `dist/windows-signed/` 内の実行ごとに異なるフォルダーとし、
以前の無署名インストーラーとの取り違えを防ぐ。

2026-09-23 時点で Release ビルド、無署名インストーラー作成、インストール版の
マーカーを付けた実行ファイルの起動とユーザー別データ作成を確認した。
署名失敗時に Inno Setup がインストーラーを生成せず停止することも確認した。
旧英語初期設定のインストーラーは実証明書で署名し、Setup の署名が `Valid` と
判定されることを確認した。日本語初期設定を含む現行のインストーラーの署名と、
インストール後のアンインストーラー署名は未検証。
