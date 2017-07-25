// Copyright 2017 Alejandro Sirgo Rica
//
// This file is part of Flameshot.
//
//     Flameshot is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     Flameshot is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with Flameshot.  If not, see <http://www.gnu.org/licenses/>.

#include "confighandler.h"
#include <QSettings>

ConfigHandler::ConfigHandler(QObject *parent) : QObject(parent) {
    m_settings = new QSettings(this);
}

QList<CaptureButton::ButtonType> ConfigHandler::getButtons() {
    QList<int> buttons = m_settings->value("buttons").value<QList<int> >();
    bool modified = normalizeButtons(buttons);
    if (modified) {
        m_settings->setValue("buttons", QVariant::fromValue(buttons));
    }
    return fromIntToButton(buttons);
}

void ConfigHandler::setButtons(const QList<CaptureButton::ButtonType> &buttons) {
    QList<int> l = fromButtonToInt(buttons);
    normalizeButtons(l);
    m_settings->setValue("buttons", QVariant::fromValue(l));
}

QString ConfigHandler::getSavePath() {
    return m_settings->value("savePath").toString();
}

void ConfigHandler::setSavePath(const QString &savePath) {
    m_settings->setValue("savePath", savePath);
}

QColor ConfigHandler::getUIMainColor() {
    return m_settings->value("uiColor").value<QColor>();
}

void ConfigHandler::setUIMainColor(const QColor &c) {
    m_settings->setValue("uiColor", c);
}

QColor ConfigHandler::getUIContrastColor() {
    return m_settings->value("contastUiColor").value<QColor>();
}

void ConfigHandler::setUIContrastColor(const QColor &c) {
    m_settings->setValue("contastUiColor", c);
}

QColor ConfigHandler::getDrawColor() {
    return m_settings->value("drawColor").value<QColor>();
}

void ConfigHandler::setDrawColor(const QColor &c) {
    m_settings->setValue("drawColor", c);
}

bool ConfigHandler::getShowHelp() {
    return m_settings->value("showHelp").toBool();
}

void ConfigHandler::setShowHelp(const bool showHelp) {
    m_settings->setValue("showHelp", showHelp);
}

bool ConfigHandler::getDesktopNotification() {
    return m_settings->value("showDesktopNotification").toBool();
}

void ConfigHandler::setDesktopNotification(const bool showDesktopNotification) {
    m_settings->setValue("showDesktopNotification", showDesktopNotification);
}

QString ConfigHandler::getFilenamePattern() {
    return m_settings->value("filenamePattern").toString();
}

void ConfigHandler::setFilenamePattern(const QString &pattern) {
    return m_settings->setValue("filenamePattern", pattern);
}

bool ConfigHandler::initiatedIsSet() {
    return m_settings->value("initiated").toBool();
}

void ConfigHandler::setInitiated() {
    m_settings->setValue("initiated", true);
}

void ConfigHandler::setNotInitiated() {
    m_settings->setValue("initiated", false);
}

void ConfigHandler::setDefaults() {
    setShowHelp(true);
    setDesktopNotification(true);
    setDrawColor(QColor(Qt::red));
    setUIMainColor(QColor(116, 0, 150));
    setUIContrastColor(QColor(86, 0, 120));
    setAllTheButtons();
}

void ConfigHandler::setAllTheButtons() {
    QList<int> buttons;
    auto listTypes = CaptureButton::getIterableButtonTypes();
    for (CaptureButton::ButtonType t: listTypes) {
        buttons << static_cast<int>(t);
    }
    m_settings->setValue("buttons", QVariant::fromValue(buttons));
}

bool ConfigHandler::normalizeButtons(QList<int> &buttons) {
    auto listTypes = CaptureButton::getIterableButtonTypes();
    QList<int> listTypesInt;
    for(auto i: listTypes) listTypesInt << static_cast<int>(i);

    bool hasChanged = false;
    QMutableListIterator<int> i(buttons);
    while (i.hasNext()) {
        if (!listTypesInt.contains(i.next())) {
            i.remove();
            hasChanged = true;
        }
    }
    return hasChanged;
}

QList<CaptureButton::ButtonType> ConfigHandler::fromIntToButton(
        const QList<int> &l)
{
    QList<CaptureButton::ButtonType> buttons;
    for(auto i: l)
        buttons << static_cast<CaptureButton::ButtonType>(i);
    return buttons;
}

QList<int> ConfigHandler::fromButtonToInt(
        const QList<CaptureButton::ButtonType> &l)
{
    QList<int> buttons;
    for(auto i: l)
        buttons << static_cast<int>(i);
    return buttons;
}