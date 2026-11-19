"""Tests for the Zigbee validation on nRF52 and nRF54L."""

from collections.abc import Generator
from typing import Any

import pytest

from esphome.components.nrf52.const import (
    VARIANT_NRF54L15,
    VARIANT_NRF54LM20,
    VARIANT_NRF52840,
)
from esphome.components.nrf52.framework import set_nrf52_variant
from esphome.components.zigbee.zigbee_zephyr import final_validate_zephyr
import esphome.config_validation as cv
from esphome.const import KEY_CORE, KEY_FRAMEWORK_VERSION, Toolchain
from esphome.core import CORE
import esphome.final_validate as fv


@pytest.fixture
def full_config() -> Generator[dict[str, Any]]:
    config: dict[str, Any] = {}
    token = fv.full_config.set(config)
    yield config
    fv.full_config.reset(token)


def _setup(
    variant: str, version: str, toolchain: Toolchain = Toolchain.SDK_NRF
) -> None:
    CORE.data[KEY_CORE] = {KEY_FRAMEWORK_VERSION: cv.Version.parse(version)}
    CORE.toolchain = toolchain
    set_nrf52_variant(variant)


@pytest.mark.parametrize(
    ("variant", "version"),
    [
        (VARIANT_NRF52840, "2.6.1"),
        (VARIANT_NRF52840, "2.9.2"),
        (VARIANT_NRF54L15, "3.4.2"),
        (VARIANT_NRF54LM20, "3.4.0"),
        (VARIANT_NRF54LM20, "3.4.1"),
    ],
)
@pytest.mark.usefixtures("full_config")
def test_supported(variant: str, version: str) -> None:
    _setup(variant, version)
    assert final_validate_zephyr({}) == {}


@pytest.mark.parametrize(
    ("variant", "version", "toolchain", "message"),
    [
        pytest.param(
            VARIANT_NRF54L15,
            "2.9.2",
            Toolchain.SDK_NRF,
            "Zigbee on NRF54L15 needs sdk-nrf 3.4.0, 3.4.1, 3.4.2; set 'version: 3.4.2'",
            id="nrf54l_on_sdk_2",
        ),
        pytest.param(
            VARIANT_NRF52840,
            "3.4.2",
            Toolchain.SDK_NRF,
            "Zigbee on NRF52840 needs sdk-nrf 2.9.2 or older",
            id="nrf52_on_sdk_3",
        ),
        pytest.param(
            VARIANT_NRF54L15,
            "3.4.2",
            Toolchain.PLATFORMIO,
            "needs 'toolchain: sdk-nrf'",
            id="platformio",
        ),
        pytest.param(
            VARIANT_NRF54L15,
            "3.3.0",
            Toolchain.SDK_NRF,
            "Zigbee is not available for sdk-nrf 3.3.0",
            id="no_add_on_release",
        ),
    ],
)
@pytest.mark.usefixtures("full_config")
def test_unsupported(
    variant: str,
    version: str,
    toolchain: Toolchain,
    message: str,
) -> None:
    _setup(variant, version, toolchain)
    with pytest.raises(cv.Invalid, match=message):
        final_validate_zephyr({})


def test_mcumgr_ota_rejected(full_config: dict[str, Any]) -> None:
    """The add-on's flash layout has no MCUboot or second image slot."""
    _setup(VARIANT_NRF54L15, "3.4.2")
    full_config["ota"] = [{"platform": "zephyr_mcumgr"}]
    with pytest.raises(cv.Invalid, match="zephyr_mcumgr OTA"):
        final_validate_zephyr({})
