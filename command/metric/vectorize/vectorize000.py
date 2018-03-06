#!/usr/bin/env python3
import sys

import driver

########################################

def get_vectorize_status(bb):
    # TODO
    return "S"

class VectorizeMetricWorker(driver.MetricWorker):
    def match_p(self, target_config, test_name):
        if target_config.compiler == "ClangLLVM":
            return True
        return False

    def set_up_before_getting_data(self, target_config, bb_list):
        pass

    def get_column_name_list(self):
        return ["vectorize"]

    def get_data_list(self, target_config, bb):
        status = get_vectorize_status(bb)
        return [status]

    def get_summary_list(self, target_config):
        return ["-"]

if __name__ == "__main__":
    vectorize_metric_worker = VectorizeMetricWorker()
    driver.run_metric_work_if_match_p(sys.argv, vectorize_metric_worker)
