/*
** ETIB PROJECT, 2026
** evan
** File description:
** InteractionProfile
*/

#include "evan/openxr/InteractionProfile.hpp"

////////////////////
// Public Methods //
////////////////////

std::string evan::InteractionProfile::getRuntimeName(XrInstance instance)
{
	XrInstanceProperties instanceProperties { XR_TYPE_INSTANCE_PROPERTIES };

	XrResult result = xrGetInstanceProperties(instance, &instanceProperties);
	if (result != XR_SUCCESS) {
		this->getLogger().error("Failed to get OpenXR instance properties: " + std::to_string(result));
		return {};
	}
	return std::string(instanceProperties.runtimeName);
}

std::string evan::InteractionProfile::getSystemName(XrInstance instance,
													XrSystemId systemId)
{
	XrSystemProperties systemProperties { XR_TYPE_SYSTEM_PROPERTIES };

	XrResult result =
		xrGetSystemProperties(instance, systemId, &systemProperties);
	if (result != XR_SUCCESS) {
		this->getLogger().error("Failed to get OpenXR system properties: " + std::to_string(result));
		return {};
	}
	return std::string(systemProperties.systemName);
}

XrPath evan::InteractionProfile::stringToPath(XrInstance instance,
											  const std::string &pathString)
{
	XrPath path = XR_NULL_PATH;

	XrResult result = xrStringToPath(instance, pathString.c_str(), &path);
	if (result != XR_SUCCESS) {
		this->getLogger().error("Failed to convert string to OpenXR path ('" + pathString + "'): " + std::to_string(result));
		return XR_NULL_PATH;
	}
	return path;
}

std::string evan::InteractionProfile::pathToString(XrInstance instance,
												   XrPath path)
{
	if (path == XR_NULL_PATH) {
		return {};
	}

	uint32_t pathStringSize = 0;
	XrResult result =
		xrPathToString(instance, path, 0, &pathStringSize, nullptr);
	if (result != XR_SUCCESS || pathStringSize == 0) {
		this->getLogger().error("Failed to get OpenXR path string size: " + std::to_string(result));
		return {};
	}

	std::string pathString(pathStringSize, '\0');
	result = xrPathToString(instance, path, pathStringSize, &pathStringSize,
							pathString.data());
	if (result != XR_SUCCESS) {
		this->getLogger().error("Failed to convert OpenXR path to string: " + std::to_string(result));
		return {};
	}

	if (!pathString.empty() && pathString.back() == '\0') {
		pathString.pop_back();
	}
	return pathString;
}

std::string evan::InteractionProfile::getCurrentInteractionProfilePath(
	XrInstance instance, XrSession session, const std::string &userPathString)
{
	if (session == XR_NULL_HANDLE) {
		this->getLogger().error("Invalid OpenXR session handle");
		return {};
	}

	XrPath userPath = stringToPath(instance, userPathString);
	if (userPath == XR_NULL_PATH) {
		return {};
	}

	XrInteractionProfileState state { XR_TYPE_INTERACTION_PROFILE_STATE };
	XrResult result = xrGetCurrentInteractionProfile(session, userPath, &state);
	if (result != XR_SUCCESS) {
		this->getLogger().error("Failed to get current interaction profile for '" + userPathString + "': " + std::to_string(result));
		return {};
	}

	if (state.interactionProfile == XR_NULL_PATH) {
		return {};
	}
	return pathToString(instance, state.interactionProfile);
}

std::vector<XrPath> evan::InteractionProfile::enumerateBoundSourcesForAction(
	XrInstance instance, XrSession session, XrAction action)
{
	this->getLogger().info("Enumerating bound sources for action");

	std::vector<XrPath> paths;

	if (session == XR_NULL_HANDLE || action == XR_NULL_HANDLE) {
		return paths;
	}

	XrBoundSourcesForActionEnumerateInfo enumerateInfo {
		XR_TYPE_BOUND_SOURCES_FOR_ACTION_ENUMERATE_INFO
	};
	enumerateInfo.action = action;

	uint32_t sourceCount = 0;
	XrResult result		 = xrEnumerateBoundSourcesForAction(
		 session, &enumerateInfo, 0, &sourceCount, nullptr);
	if (result != XR_SUCCESS || sourceCount == 0) {
		return paths;
	}

	paths.resize(sourceCount, XR_NULL_PATH);
	result = xrEnumerateBoundSourcesForAction(
		session, &enumerateInfo, sourceCount, &sourceCount, paths.data());
	if (result != XR_SUCCESS) {
		this->getLogger().error("Failed to enumerate bound sources for action: " + std::to_string(result));
		return {};
	}

	return paths;
}

std::vector<std::string>
	evan::InteractionProfile::enumerateBoundSourcePathStringsForAction(
		XrInstance instance, XrSession session, XrAction action)
{
	this->getLogger().info("Enumerating bound source path strings for action");

	std::vector<std::string> sourcePathStrings;
	std::vector<XrPath> sourcePaths =
		enumerateBoundSourcesForAction(instance, session, action);

	sourcePathStrings.reserve(sourcePaths.size());
	for (XrPath sourcePath: sourcePaths) {
		sourcePathStrings.emplace_back(pathToString(instance, sourcePath));
	}
	return sourcePathStrings;
}