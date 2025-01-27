#
# Copyright (c) 2012-2014 Microsoft Mobile and/or its subsidiary(-ies).
# All rights reserved.
#
# For the applicable distribution terms see the license text file included in
# the distribution.
#

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../../

contains(DEFINES,COCOS2DXQT_LIBRARY) {
!contains(DEFINES,DONT_INCLUDE_BOX2D_SOURCES) {
    SOURCES += \
        $$PWD/../Rope/b2Rope.cpp \
        $$PWD/../Dynamics/b2Fixture.cpp \
        $$PWD/../Dynamics/b2WorldCallbacks.cpp \
        $$PWD/../Dynamics/b2ContactManager.cpp \
        $$PWD/../Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
        $$PWD/../Dynamics/Contacts/b2Contact.cpp \
        $$PWD/../Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
        $$PWD/../Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
        $$PWD/../Dynamics/Contacts/b2PolygonContact.cpp \
        $$PWD/../Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
        $$PWD/../Dynamics/Contacts/b2ContactSolver.cpp \
        $$PWD/../Dynamics/Contacts/b2ChainAndCircleContact.cpp \
        $$PWD/../Dynamics/Contacts/b2CircleContact.cpp \
        $$PWD/../Dynamics/Joints/b2MouseJoint.cpp \
        $$PWD/../Dynamics/Joints/b2WheelJoint.cpp \
        $$PWD/../Dynamics/Joints/b2PulleyJoint.cpp \
        $$PWD/../Dynamics/Joints/b2RopeJoint.cpp \
        $$PWD/../Dynamics/Joints/b2FrictionJoint.cpp \
        $$PWD/../Dynamics/Joints/b2RevoluteJoint.cpp \
        $$PWD/../Dynamics/Joints/b2WeldJoint.cpp \
        $$PWD/../Dynamics/Joints/b2DistanceJoint.cpp \
        $$PWD/../Dynamics/Joints/b2GearJoint.cpp \
        $$PWD/../Dynamics/Joints/b2PrismaticJoint.cpp \
        $$PWD/../Dynamics/Joints/b2Joint.cpp \
        $$PWD/../Dynamics/b2Body.cpp \
        $$PWD/../Dynamics/b2Island.cpp \
        $$PWD/../Dynamics/b2World.cpp \
        $$PWD/../Common/b2BlockAllocator.cpp \
        $$PWD/../Common/b2Timer.cpp \
        $$PWD/../Common/b2Math.cpp \
        $$PWD/../Common/b2Settings.cpp \
        $$PWD/../Common/b2StackAllocator.cpp \
        $$PWD/../Common/b2Draw.cpp \
        $$PWD/../Collision/b2CollideEdge.cpp \
        $$PWD/../Collision/b2TimeOfImpact.cpp \
        $$PWD/../Collision/b2BroadPhase.cpp \
        $$PWD/../Collision/b2CollideCircle.cpp \
        $$PWD/../Collision/Shapes/b2PolygonShape.cpp \
        $$PWD/../Collision/Shapes/b2EdgeShape.cpp \
        $$PWD/../Collision/Shapes/b2CircleShape.cpp \
        $$PWD/../Collision/Shapes/b2ChainShape.cpp \
        $$PWD/../Collision/b2CollidePolygon.cpp \
        $$PWD/../Collision/b2Collision.cpp \
        $$PWD/../Collision/b2Distance.cpp \
        $$PWD/../Collision/b2DynamicTree.cpp
}
}

HEADERS = \
    $$PWD/../Rope/b2Rope.h \
    $$PWD/../Dynamics/b2World.h \
    $$PWD/../Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    $$PWD/../Dynamics/Contacts/b2PolygonAndCircleContact.h \
    $$PWD/../Dynamics/Contacts/b2ContactSolver.h \
    $$PWD/../Dynamics/Contacts/b2CircleContact.h \
    $$PWD/../Dynamics/Contacts/b2ChainAndPolygonContact.h \
    $$PWD/../Dynamics/Contacts/b2ChainAndCircleContact.h \
    $$PWD/../Dynamics/Contacts/b2PolygonContact.h \
    $$PWD/../Dynamics/Contacts/b2Contact.h \
    $$PWD/../Dynamics/Contacts/b2EdgeAndCircleContact.h \
    $$PWD/../Dynamics/b2TimeStep.h \
    $$PWD/../Dynamics/b2WorldCallbacks.h \
    $$PWD/../Dynamics/Joints/b2PrismaticJoint.h \
    $$PWD/../Dynamics/Joints/b2WheelJoint.h \
    $$PWD/../Dynamics/Joints/b2WeldJoint.h \
    $$PWD/../Dynamics/Joints/b2RevoluteJoint.h \
    $$PWD/../Dynamics/Joints/b2DistanceJoint.h \
    $$PWD/../Dynamics/Joints/b2RopeJoint.h \
    $$PWD/../Dynamics/Joints/b2MouseJoint.h \
    $$PWD/../Dynamics/Joints/b2GearJoint.h \
    $$PWD/../Dynamics/Joints/b2PulleyJoint.h \
    $$PWD/../Dynamics/Joints/b2FrictionJoint.h \
    $$PWD/../Dynamics/Joints/b2Joint.h \
    $$PWD/../Dynamics/b2Fixture.h \
    $$PWD/../Dynamics/b2Island.h \
    $$PWD/../Dynamics/b2Body.h \
    $$PWD/../Dynamics/b2ContactManager.h \
    $$PWD/../Common/b2Draw.h \
    $$PWD/../Common/b2StackAllocator.h \
    $$PWD/../Common/b2BlockAllocator.h \
    $$PWD/../Common/b2Settings.h \
    $$PWD/../Common/b2GrowableStack.h \
    $$PWD/../Common/b2Math.h \
    $$PWD/../Common/b2Timer.h \
    $$PWD/../Collision/b2Collision.h \
    $$PWD/../Collision/Shapes/b2Shape.h \
    $$PWD/../Collision/Shapes/b2ChainShape.h \
    $$PWD/../Collision/Shapes/b2PolygonShape.h \
    $$PWD/../Collision/Shapes/b2CircleShape.h \
    $$PWD/../Collision/Shapes/b2EdgeShape.h \
    $$PWD/../Collision/b2DynamicTree.h \
    $$PWD/../Collision/b2Distance.h \
    $$PWD/../Collision/b2BroadPhase.h \
    $$PWD/../Collision/b2TimeOfImpact.h \
    $$PWD/../Box2D.h
