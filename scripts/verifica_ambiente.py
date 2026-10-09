"""
Roda antes de todo build (extra_scripts = pre:...) e impede dois problemas que
quebram o projeto quando o PlatformIO global da maquina muda:

1. Cache do CMake velho: o .pio/build/<env> guarda o caminho absoluto do ESP-IDF.
   Se outro projeto/extensao reinstalar outra versao nesse caminho, o build mistura
   duas versoes do ESP-IDF ("fragment not found", "Multiple ways to build...").
2. sdkconfig.<env> congelado: o ESP-IDF so aplica o sdkconfig.defaults em chaves que
   ainda nao existem no sdkconfig.<env> gerado, entao mudar o .defaults nao tinha efeito.

Os dois sao arquivos gerados (ignorados pelo git), entao apagar e seguro. O script
nunca derruba o build: no maximo avisa e segue.
"""

import hashlib
import json
import shutil
from pathlib import Path

Import("env")  # noqa: F821  (injetado pelo SCons)


def _sha256(arquivo: Path) -> str:
    return hashlib.sha256(arquivo.read_bytes()).hexdigest() if arquivo.is_file() else "ausente"


def _tem_conteudo(pasta_build: Path) -> bool:
    # o PlatformIO cria a pasta vazia antes de rodar este script
    return pasta_build.is_dir() and any(not i.name.startswith(".sconsign") for i in pasta_build.iterdir())


def _limpar_build(pasta_build: Path) -> None:
    if not pasta_build.is_dir():
        return
    for item in pasta_build.iterdir():
        if item.name.startswith(".sconsign"):  # banco interno do SCons, em uso agora
            continue
        if item.is_dir():
            shutil.rmtree(item, ignore_errors=True)
        else:
            item.unlink(missing_ok=True)


def _verificar() -> None:
    projeto = Path(env["PROJECT_DIR"])
    nome_env = env["PIOENV"]
    pasta_build = Path(env.subst("$BUILD_DIR"))
    plataforma = env.PioPlatform()

    framework_dir = plataforma.get_package_dir("framework-espidf")
    if not framework_dir:  # primeira execucao, pacotes ainda sendo instalados
        return

    atual = {
        "framework_dir": str(Path(framework_dir).resolve()),
        "framework_versao": str(plataforma.get_package_version("framework-espidf")),
        "sdkconfig_defaults": _sha256(projeto / "sdkconfig.defaults"),
    }

    arquivo_estado = pasta_build.parent / f"{nome_env}.ambiente.json"
    try:
        anterior = json.loads(arquivo_estado.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        anterior = None  # sem registro: nao da para confiar no cache existente

    if anterior != atual:
        sdkconfig_env = projeto / f"sdkconfig.{nome_env}"
        existe_cache = _tem_conteudo(pasta_build) or sdkconfig_env.is_file()

        if existe_cache:
            if anterior is None:
                motivo = "primeira verificacao deste ambiente"
            elif anterior["framework_dir"] != atual["framework_dir"] or anterior["framework_versao"] != atual["framework_versao"]:
                motivo = "o ESP-IDF instalado mudou"
            else:
                motivo = "o sdkconfig.defaults mudou"
            print(f"[ambiente] {nome_env}: {motivo}; regenerando cache de build e {sdkconfig_env.name}")
            _limpar_build(pasta_build)
            sdkconfig_env.unlink(missing_ok=True)

    arquivo_estado.parent.mkdir(parents=True, exist_ok=True)
    arquivo_estado.write_text(json.dumps(atual, indent=2), encoding="utf-8")


try:
    _verificar()
except Exception as erro:  # nunca impede o build por causa da verificacao
    print(f"[ambiente] aviso: verificacao ignorada ({erro})")
