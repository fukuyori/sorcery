# バージョン更新チェックリスト

フォークのバージョンを変更するときに、現在のソースで確認する場所をまとめる。
upstream のバージョンとフォークのリリース番号を混同しない。

## ソースと表示

- [ ] `CMakeLists.txt` の `project(sorcery VERSION ...)` を更新する。
- [ ] 同じファイルの `SORCERY_BUILD_LABEL` をリリース表示に合わせる。
  タイトル画面右下にはこのラベルとビルド日が表示される。
- [ ] `inc/generated/version.hpp.in` が `PROJECT_VERSION`、
  `SORCERY_BUILD_LABEL`、`SORCERY_BUILD_DATE` を参照していることを確認する。
  通常はテンプレートの値を直接編集しない。
- [ ] `CMakeLists.txt` の `CPACK_PROJECT_VERSION` が `PROJECT_VERSION` を
  参照していることを確認する。

## 文書

- [ ] `CHANGELOG.md` の `[Unreleased]` から今回の変更をリリース見出しへ移し、
  次の `[Unreleased]` を用意する。
- [ ] `README.md` のフォーク側の説明と `README.ja.md` にあるリリース状況を
  更新する。`README.md` の区切り線以降に転載した upstream README は
  フォークのリリース情報で書き換えない。
- [ ] `docs/FORK_DESIGN.ja.md`、`docs/ja-font.md`、その他のフォーク文書に
  古いバージョンや検証結果が残っていないか確認する。

## ビルドと配布

- [ ] Release ビルドを作成し、実行ファイルの表示ラベルと起動を確認する。
- [ ] 配布用スクリプトや Inno Setup の設定を整備した後は、そのスクリプトにある
  アプリ版数、インストーラー版数、出力ファイル名も確認する。
- [ ] 署名付き配布を行う場合は `-Sign` と `CODESIGN_CERT` を使用し、実行ファイル、
  インストーラー、アンインストーラーの署名を確認する。
- [ ] タグ、配布物、変更履歴の版数が一致することを確認する。

現在、Release ビルドと Inno Setup のスクリプトは未整備。作成時に具体的な
ファイル名と確認手順をこのチェックリストへ追加する。
