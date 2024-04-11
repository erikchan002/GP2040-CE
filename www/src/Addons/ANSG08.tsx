import React from 'react';
import { useTranslation } from 'react-i18next';
import { FormCheck, Row } from 'react-bootstrap';
import * as yup from 'yup';

import Section from '../Components/Section';
import FormSelect from '../Components/FormSelect';

import FormControl from '../Components/FormControl';
import { I2C_BLOCKS } from '../Data/Peripherals';

export const ansg08Scheme = {
    I2CANSG08InputEnabled: yup.number().label('I2C ANSG08 Input Enabled'),
    i2cANSG08SDAPin: yup
        .number()
        .label('I2C ANSG08 SDA Pin')
        .validatePinWhenValue('I2CANSG08InputEnabled'),
    i2cANSG08SCLPin: yup
        .number()
        .label('I2C ANSG08 SCL Pin')
        .validatePinWhenValue('I2CANSG08InputEnabled'),
    i2cANSG08Block: yup
        .number()
        .label('I2C ANSG08 Block')
        .validateSelectionWhenValue('I2CANSG08InputEnabled', I2C_BLOCKS),
    i2cANSG08Speed: yup
        .number()
        .label('I2C ANSG08 Speed')
        .validateNumberWhenValue('I2CANSG08InputEnabled'),
    i2cANSG08Address0: yup
        .number()
        .label('I2C first ANSG08 Address')
        .validateNumberWhenValue('I2CANSG08InputEnabled'),
    i2cANSG08Address1: yup
        .number()
        .label('I2C second ANSG08 Address')
        .validateNumberWhenValue('I2CANSG08InputEnabled'),
    i2cANSG08Address2: yup
        .number()
        .label('I2C third ANSG08 Address')
        .validateNumberWhenValue('I2CANSG08InputEnabled'),
    i2cANSG08Address3: yup
        .number()
        .label('I2C fourth ANSG08 Address')
        .validateNumberWhenValue('I2CANSG08InputEnabled'),
};

export const ansg08State = {
    I2CANSG08InputEnabled: 0,
    i2cANSG08SDAPin: -1,
    i2cANSG08SCLPin: -1,
    i2cANSG08Block: 0,
    i2cANSG08Speed: 400000,
    i2cANSG08Address0: 0x20,
    i2cANSG08Address1: 0x21,
    i2cANSG08Address2: 0x22,
    i2cANSG08Address3: 0x23,
};

const ANSG08 = ({ values, errors, handleChange, handleCheckbox }) => {
    const { t } = useTranslation();
    return (
        <Section title={t('AddonsConfig:i2c-ansg08-header-text')}>
            <div
                id="i2cANSG08InputOptions"
                hidden={!values.I2CANSG08InputEnabled}
            >
                <Row className="mb-3">
                    <FormControl
                        type="number"
                        label={t('AddonsConfig:i2c-ansg08-sda-pin-label')}
                        name="i2cANSG08SDAPin"
                        className="form-control-sm"
                        groupClassName="col-sm-3 mb-3"
                        value={values.i2cANSG08SDAPin}
                        error={errors.i2cANSG08SDAPin}
                        isInvalid={errors.i2cANSG08SDAPin}
                        onChange={handleChange}
                        min={-1}
                        max={29}
                    />
                    <FormControl
                        type="number"
                        label={t('AddonsConfig:i2c-ansg08-scl-pin-label')}
                        name="i2cANSG08SCLPin"
                        className="form-select-sm"
                        groupClassName="col-sm-3 mb-3"
                        value={values.i2cANSG08SCLPin}
                        error={errors.i2cANSG08SCLPin}
                        isInvalid={errors.i2cANSG08SCLPin}
                        onChange={handleChange}
                        min={-1}
                        max={29}
                    />
                    <FormSelect
                        label={t('AddonsConfig:i2c-ansg08-block-label')}
                        name="i2cANSG08Block"
                        className="form-select-sm"
                        groupClassName="col-sm-3 mb-3"
                        value={values.i2cANSG08Block}
                        error={errors.i2cANSG08Block}
                        isInvalid={errors.i2cANSG08Block}
                        onChange={handleChange}
                    >
                        {I2C_BLOCKS.map((o, i) => (
                            <option key={`i2cBlock-option-${i}`} value={o.value}>
                                {o.label}
                            </option>
                        ))}
                    </FormSelect>
                    <FormControl
                        label={t('AddonsConfig:i2c-ansg08-speed-label')}
                        name="i2cANSG08Speed"
                        className="form-control-sm"
                        groupClassName="col-sm-3 mb-3"
                        value={values.i2cANSG08Speed}
                        error={errors.i2cANSG08Speed}
                        isInvalid={errors.i2cANSG08Speed}
                        onChange={handleChange}
                        min={100000}
                    />
                </Row>
                <Row className="mb-3">
                    <FormControl
                        label={t('AddonsConfig:i2c-ansg08-address-0-label')}
                        name="i2cANSG08Address0"
                        className="form-control-sm"
                        groupClassName="col-sm-3 mb-3"
                        value={values.i2cANSG08Address0}
                        error={errors.i2cANSG08Address0}
                        isInvalid={errors.i2cANSG08Address0}
                        onChange={handleChange}
                        maxLength={4}
                    />
                </Row>
                <Row className="mb-3">
                    <FormControl
                        label={t('AddonsConfig:i2c-ansg08-address-1-label')}
                        name="i2cANSG08Address1"
                        className="form-control-sm"
                        groupClassName="col-sm-3 mb-3"
                        value={values.i2cANSG08Address1}
                        error={errors.i2cANSG08Address1}
                        isInvalid={errors.i2cANSG08Address1}
                        onChange={handleChange}
                        maxLength={4}
                    />
                </Row>
                <Row className="mb-3">
                    <FormControl
                        label={t('AddonsConfig:i2c-ansg08-address-2-label')}
                        name="i2cANSG08Address2"
                        className="form-control-sm"
                        groupClassName="col-sm-3 mb-3"
                        value={values.i2cANSG08Address2}
                        error={errors.i2cANSG08Address2}
                        isInvalid={errors.i2cANSG08Address2}
                        onChange={handleChange}
                        maxLength={4}
                    />
                </Row>
                <Row className="mb-3">
                    <FormControl
                        label={t('AddonsConfig:i2c-ansg08-address-3-label')}
                        name="i2cANSG08Address3"
                        className="form-control-sm"
                        groupClassName="col-sm-3 mb-3"
                        value={values.i2cANSG08Address3}
                        error={errors.i2cANSG08Address3}
                        isInvalid={errors.i2cANSG08Address3}
                        onChange={handleChange}
                        maxLength={4}
                    />
                </Row>
            </div>
            <FormCheck
                label={t('Common:switch-enabled')}
                type="switch"
                id="i2cANSG08InputButton"
                reverse
                isInvalid={false}
                checked={Boolean(values.I2CANSG08InputEnabled)}
                onChange={(e) => {
                    handleCheckbox('I2CANSG08InputEnabled', values);
                    handleChange(e);
                }}
            />
        </Section>
    );
};

export default ANSG08;
