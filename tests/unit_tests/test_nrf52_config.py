"""Tests for the nRF52 configuration validation."""

import pytest

from esphome.components.nrf52 import (
    _detect_bootloader,
    _detect_variant,
    get_nrf52_variant,
    only_on_variant,
)
from esphome.components.nrf52.const import VARIANT_NRF52, VARIANT_NRF54L
from esphome.components.nrf52.framework import _get_data
import esphome.config_validation as cv
from esphome.const import (
    CONF_BOARD,
    CONF_VARIANT,
    KEY_CORE,
    KEY_TARGET_PLATFORM,
    PLATFORM_NRF52,
)
from esphome.core import CORE


def test_detect_bootloader_reports_a_missing_board() -> None:
    """The bootloader check runs before the schema, so it reports the missing key."""
    with pytest.raises(cv.Invalid, match="'board' is a required option"):
        _detect_bootloader({})


@pytest.mark.parametrize(
    ("board", "variant"),
    [
        ("nrf54l15dk/nrf54l15/cpuapp", VARIANT_NRF54L),
        ("nrf54l15dk/nrf54l10/cpuapp", VARIANT_NRF54L),
        ("xiao_ble", VARIANT_NRF52),
        ("adafruit_feather_nrf52840", VARIANT_NRF52),
    ],
)
def test_detect_variant_from_board(board: str, variant: str) -> None:
    assert _detect_variant({CONF_BOARD: board})[CONF_VARIANT] == variant


def test_detect_variant_explicit_for_unknown_board() -> None:
    config = {CONF_BOARD: "my_custom_board", CONF_VARIANT: "nrf54l"}
    assert _detect_variant(config)[CONF_VARIANT] == VARIANT_NRF54L


def test_detect_variant_mismatch() -> None:
    config = {CONF_BOARD: "nrf54l15dk/nrf54l15/cpuapp", CONF_VARIANT: VARIANT_NRF52}
    with pytest.raises(cv.Invalid, match="does not match the selected board"):
        _detect_variant(config)


def test_detect_variant_unknown_value() -> None:
    with pytest.raises(cv.Invalid):
        _detect_variant({CONF_BOARD: "xiao_ble", CONF_VARIANT: "NRF51"})


def _set_nrf52_variant(variant: str) -> None:
    CORE.data[KEY_CORE] = {KEY_TARGET_PLATFORM: PLATFORM_NRF52}
    _get_data().variant = variant


def test_only_on_variant() -> None:
    _set_nrf52_variant(VARIANT_NRF54L)
    assert get_nrf52_variant() == VARIANT_NRF54L
    with pytest.raises(cv.Invalid, match="'dfu' is only available on nRF52"):
        only_on_variant(supported=VARIANT_NRF52, msg_prefix="'dfu'")("x")
    assert only_on_variant(supported=[VARIANT_NRF54L])("x") == "x"
    with pytest.raises(cv.Invalid, match="is not available on nRF54L"):
        only_on_variant(unsupported=VARIANT_NRF54L)("x")


def test_split_default_nrf52_variant_keys() -> None:
    schema = cv.Schema(
        {cv.SplitDefault("key", nrf52="base", nrf52_nrf54l="nrf54l"): str}
    )
    _set_nrf52_variant(VARIANT_NRF54L)
    assert schema({})["key"] == "nrf54l"
    _set_nrf52_variant(VARIANT_NRF52)
    assert schema({})["key"] == "base"
