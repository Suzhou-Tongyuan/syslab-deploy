#![allow(unused_imports)]
#![allow(unused_mut)]

use super::*;
use prost::Message;
use rinf::{DartSignal, RinfError};
use std::collections::HashMap;
use std::sync::LazyLock;

type Handler = dyn Fn(&[u8], &[u8]) -> Result<(), RinfError> + Send + Sync;
type DartSignalHandlers = HashMap<i32, Box<Handler>>;
static DART_SIGNAL_HANDLERS: LazyLock<DartSignalHandlers> = LazyLock::new(|| {
    let mut hash_map: DartSignalHandlers = HashMap::new();
hash_map.insert(
    0,
    Box::new(|message_bytes: &[u8], binary: &[u8]| {
        let message =
            GenericDartSignal::decode(message_bytes)
            .map_err(|_| RinfError::CannotDecodeMessage)?;
        let dart_signal = DartSignal {
            message,
            binary: binary.to_vec(),
        };
        GENERIC_DART_SIGNAL_CHANNEL.0.send(dart_signal);
        Ok(())
    }),
);
    hash_map
});

pub fn assign_dart_signal(
    message_id: i32,
    message_bytes: &[u8],
    binary: &[u8]
) -> Result<(), RinfError> {
    let signal_handler = match DART_SIGNAL_HANDLERS.get(&message_id) {
        Some(inner) => inner,
        None => return Err(RinfError::NoSignalHandler),
    };
    signal_handler(message_bytes, binary)
}
