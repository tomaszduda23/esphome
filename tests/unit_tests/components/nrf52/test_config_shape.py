"""Malformed nrf52 blocks give a validation error, not a crash."""

import pytest

from esphome.components import nrf52
import esphome.config_validation as cv
from esphome.const import CONF_BOARD
from esphome.types import ConfigType


@pytest.mark.parametrize(
    ("config", "match"),
    [
        pytest.param("xiao_ble", "expected a dictionary", id="block_is_string"),
        pytest.param(["xiao_ble"], "expected a dictionary", id="block_is_list"),
        pytest.param({CONF_BOARD: ["a"]}, "Must be string", id="board_is_list"),
        pytest.param({CONF_BOARD: {"a": 1}}, "Must be string", id="board_is_mapping"),
        pytest.param(
            {CONF_BOARD: "nrf52dk/nrf52832", "bootloader": ["mcuboot"]},
            "string value cannot be dictionary or list",
            id="bootloader_is_list",
        ),
    ],
)
def test_malformed_block_is_invalid(config: ConfigType, match: str) -> None:
    with pytest.raises(cv.Invalid, match=match):
        nrf52.CONFIG_SCHEMA(config)


def test_bootloader_is_lowered_before_detection() -> None:
    config = nrf52._validate_shape({CONF_BOARD: "xiao_ble", "bootloader": "Adafruit"})
    assert config["bootloader"] == "adafruit"
